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

from library.IFitPlugin import IFitPlugin
from library.XmlContainer import XmlContainer
from library.utils import real_path_dir_for


class EbgPchStrapsPlugin(IFitPlugin):
    '''
        PCH Straps plugin creates binary from pch_straps.xml.
    '''
    name = "ebg_pch_straps"
    plugins_dir = real_path_dir_for(__file__)
    containers = [(XmlContainer, {'xml_name': 'ebg_pch_straps_ac.xml', 'main_dir': plugins_dir,
                                  'xmls': {'Archer City SPR': 'ebg_pch_straps_ac.xml', 'Archer City CLX': 'ebg_pch_straps_clx.xml', 'Archer City xPV SPR': 'ebg_pch_straps_xpv_spr.xml',
           								   'Archer City xPV CLX': 'ebg_pch_straps_xpv_clx.xml', 'A21': 'ebg_pch_straps_a21.xml', 'Archer City Modular': 'ebg_pch_straps_ac_mod.xml'}})]
