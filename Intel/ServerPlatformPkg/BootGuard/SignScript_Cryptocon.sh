INPUT_UNSIGN_FILE=${1}
OUTPUT_SIGN_FILE=${2}
CRYPTCON=CryptoCon

if [ "$1" = "BpmWithoutSigElement.bin" ]; then 
	KEY=BpmPEM.key
fi

if [ "$1" = "KmWithoutSigElement.bin" ]; then 
    KEY=KmPEM.key
fi

if [ "$1" = "FvWithoutSigElement.bin" ]; then 
    KEY=BpmPEM.key
fi

if [ "$3" = "2" ]; then 
    KEY=Km2.key
fi

if [ "$3" = "3" ]; then 
    KEY=Km3.key
fi

if [ "$BOOTGUARD_BUILD_TIME_SUPPORT_USING_BATCH_FILE" = "0" ]; then 
    ./ServerPlatformPkg/BootGuard/${CRYPTCON} -s -k ServerPlatformPkg/BootGuard/${KEY} -f ${INPUT_UNSIGN_FILE} -o ${OUTPUT_SIGN_FILE}
else
   ./$CRYPTCON -s -k ../../ServerPlatformPkg/BootGuard/${KEY} -f ${INPUT_UNSIGN_FILE} -o ${OUTPUT_SIGN_FILE}
fi