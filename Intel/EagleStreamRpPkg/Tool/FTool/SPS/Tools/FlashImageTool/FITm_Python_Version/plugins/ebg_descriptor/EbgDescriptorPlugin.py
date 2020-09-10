"""
INTEL CONFIDENTIAL
Copyright 2017-2020 Intel Corporation.
This software and the related documents are Intel copyrighted materials, and
your use of them is governed by the express license under which they were
provided to you (License).Unless the License provides otherwise, you may not
use, modify, copy, publish, distribute, disclose or transmit this software or
the related documents without Intel's prior written permission.

This software and the related documents are provided as is, with no express or
implied warranties, other than those that are expressly stated in the License.
"""

import os

from library.IContainer import IContainer
from library.IFitPlugin import IFitPlugin
from library.XmlContainer import XmlContainer
from library.tool.BinaryGenerator import BinaryGenerator
from library.utils import real_path_dir_for
from library.tool.exceptions import InvalidAttributeException
from library.tool.LibException import ComponentException
from library.tool.utils import get_file_name_no_ext, construct_public_key
from library.tool.structures import SupportedSHAs
from library.tool.components.IterableComponent import IterableComponent
from typing import Dict
from collections import namedtuple
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import padding, utils
from cryptography.exceptions import InvalidSignature
from library.tool.ColorPrint import ColorPrint
from library.tool.utils import calculate_hash


class DescriptorContainer(XmlContainer):

    def build(self, required_containers):
        super().build(required_containers)
        self.create_manifest_extension()
        self.generator.save(self.output_path)

    def create_manifest_extension(self):
        buffer = bytearray(self.generator.buffer)
        xml_param_val = lambda name: self.generator.configuration_root.get_child(name).value
        action_params = ['OfflineSigning', 'FdSigningKey', 'FdHashPath', 'FdManifest']
        params = {x: xml_param_val(x) for x in action_params}
        range_entries = self.generator.configuration_root.get_child("hash_excl_range_entries")
        modified_buffer = EbgDescriptorPlugin.create_manifest_extension(buffer, params, range_entries)
        self.generator.buffer[0: len(modified_buffer)] = modified_buffer


class EbgDescriptorPlugin(IFitPlugin):
    """
        Descriptor plugin create binary from descriptor.xml. It also has dynamic fields in main layout.xml
        that are filled after the actual build of the layout

        It uses Fit library
    """
    name = "ebg_descriptor"
    plugins_dir = real_path_dir_for(__file__)
    containers = [(DescriptorContainer, {'xml_name': 'ebg_descriptor.xml', 'main_dir': plugins_dir})]
    hash_excl_start = 0x800  # == signature_start
    hash_excl_end = 0xC00

    descriptor_size = 0x1000

    manifest_modulus_size_start = 0x78
    manifest_modulus_size_end = 0x7C
    manifest_exponent_size_start = manifest_modulus_size_end
    manifest_exponent_size_end = 0x80
    manifest_modulus_start = 0x80

    @staticmethod
    def _verify_manifest(sha: bytearray, manifest: bytearray):
        modulus_size = int.from_bytes(manifest[EbgDescriptorPlugin.manifest_modulus_size_start: EbgDescriptorPlugin.manifest_modulus_size_end], 'little') * 4
        exponent_size = int.from_bytes(manifest[EbgDescriptorPlugin.manifest_exponent_size_start: EbgDescriptorPlugin.manifest_exponent_size_end], 'little') * 4
        modulus = manifest[EbgDescriptorPlugin.manifest_modulus_start: EbgDescriptorPlugin.manifest_modulus_start + modulus_size]
        exponent = manifest[EbgDescriptorPlugin.manifest_modulus_start + modulus_size: EbgDescriptorPlugin.manifest_modulus_start + modulus_size + exponent_size]
        signature_start = EbgDescriptorPlugin.manifest_modulus_start + modulus_size + exponent_size
        signature_end = signature_start + modulus_size
        signature = manifest[signature_start: signature_end][::-1]
        manifest_hash_start = signature_end + 0x28
        manifest_hash = manifest[manifest_hash_start:]

        if sha != manifest_hash:
            raise InvalidAttributeException('FdManifest verification failed. Hash in manifest doesn\'t match descriptor.')

        data = manifest[: EbgDescriptorPlugin.manifest_modulus_start] + manifest[signature_end:]
        key = construct_public_key(exponent, modulus)
        sha_alg = hashes.SHA384()
        data_hash = calculate_hash(data, SupportedSHAs.ShaType.Sha384, False)

        try:
            key.verify(signature, data_hash, padding.PSS(padding.MGF1(sha_alg), 32), utils.Prehashed(sha_alg))
        except InvalidSignature:
            raise InvalidAttributeException('FdManifest verification failed. Incorrect signature.')

        ColorPrint.success('Fd signature verification success')

    @staticmethod
    def create_manifest_extension(buffer: bytearray, action_params: Dict[str, int], range_entries: IterableComponent):
        params = namedtuple("Params", action_params.keys())(*action_params.values())
        FD0V_manifest_path = os.path.join(IContainer.intermediate_directory, 'FD0V_Manifest.bin')
        extension_type_size = 4
        extension_type_value = 23
        extension_length_size = 4
        extension_length_value = 88
        exception_base_size = 2
        exception_limit_size = 2
        number_of_exceptions = 8

        if params.OfflineSigning:
            if not params.FdManifest:
                ColorPrint.warning('warning: OfflineSigning enabled, but no FdManifest provided. Image might require additional postprocessing.')
            if params.FdSigningKey:
                ColorPrint.warning("warning: FdSigningKey provided even though OfflineSigning is enabled. It will be unused.")
        elif params.FdManifest:
            ColorPrint.warning("warning: FdManifest provided even though OfflineSigning is disabled. It won't be included in binary.")

        if not os.path.exists(IContainer.intermediate_directory):
            os.mkdir(IContainer.intermediate_directory)
        with open(params.FdHashPath, 'wb') as manifest_file:
            manifest_file.write(extension_type_value.to_bytes(extension_type_size, byteorder='little'))
            manifest_file.write(extension_length_value.to_bytes(extension_length_size, byteorder='little'))
            buffer_cpy = buffer.copy()
            for entry in range_entries.entries:
                base = entry.children_by_name['exclude_base'].value
                limit = entry.children_by_name['exclude_limit'].value
                if base > limit:
                    raise InvalidAttributeException(f'Range limit cannot be smaller than base. '
                                                    f'"Entry index": "{entry.index}", '
                                                    f'"exclude_base": "{base}", '
                                                    f'"exclude_limit": "{limit}"')
                if limit > EbgDescriptorPlugin.descriptor_size:
                    raise InvalidAttributeException(f'Range limit cannot exceed flash descriptor region size. '
                                                    f'"Entry index": "{entry.index}", '
                                                    f'"exclude_limit": "{limit}", '
                                                    f'region size: {EbgDescriptorPlugin.descriptor_size}')
                manifest_file.write(base.to_bytes(exception_base_size, byteorder='little'))
                manifest_file.write(limit.to_bytes(exception_limit_size, byteorder='little'))
                buffer_cpy[base:limit] = [0xff] * (limit - base)
            manifest_file.write(b'\x00\x00\x00\x00' * (number_of_exceptions - range_entries.entries_count))
            sha = calculate_hash(buffer_cpy, SupportedSHAs.ShaType.Sha384, False)
            manifest_file.write(sha)
        if params.OfflineSigning == 0 and params.FdSigningKey:
            generator = BinaryGenerator(os.path.join(EbgDescriptorPlugin.plugins_dir, 'FD0V_Manifest.xml'))
            options = namedtuple("Options", ['setting_overrides', 'output_file', 'output_info', 'output_map', 'skip_validation'])
            options.setting_overrides = ['extension_binary=' + params.FdHashPath, 'key=' + params.FdSigningKey]
            options.output_info = None
            options.output_map = None
            options.output_file = FD0V_manifest_path
            try:
                generator.process_build(options, get_file_name_no_ext(params.FdHashPath))
            except ComponentException as e:
                if e.args[0] == 'Public key detected for signature':
                    raise ComponentException('Public key given, but private key expected.', 'FdSigningKey')
                else:
                    raise e
            buffer[EbgDescriptorPlugin.hash_excl_start: EbgDescriptorPlugin.hash_excl_start + generator.buffer.max_size] = generator.buffer
        elif params.OfflineSigning and params.FdManifest:
            with open(params.FdManifest, 'rb') as file:
                manifest = bytearray(file.read())
                EbgDescriptorPlugin._verify_manifest(sha, manifest)
        return buffer
