#!/bin/bash
#***********************************************************************
#*                                                                     *
#*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
#*                                                                     *
#*      All rights reserved. Subject to AMI licensing agreement.       *
#*                                                                     *
#***********************************************************************
echo 
echo DsmAsl, Generating PciSlot.asl
echo
sed 's/(/ /g; s/),/ /g' $1 > Build/DsmAsl.tmp
while IFS="," read -r a b c 
do
if [[ $c != "" ]];then
cat <<EOT >> Build/PciSlot.asl
Scope($c){
        Method ( _DSM, 4, NotSerialized, 0, UnknownObj, {BuffObj, IntObj, IntObj,PkgObj})
        {
           If (LEqual(Arg0, ToUUID ("E5C937D0-3553-4d7a-9117-EA4D19C3434D"))) {
              If (Land( Lequal(Arg1, 2), Lequal(Arg2,0))){
                 return (Buffer() {0x80})
              }
              If (Land( Lequal(Arg1, 2), Lequal(Arg2,7))){
                 return ( Package(2){
                           $a,
                         Unicode("$b")
                        }
                        )
              }
           }
           return(Buffer(){0})
        }
}
EOT
fi
done < Build/DsmAsl.tmp
rm -f Build/DsmAsl.tmp
