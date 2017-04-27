# make clean
# make mrproper
KERNEL_DIR=$PWD
export ARCH=arm
export CROSS_COMPILE=/home/$USER/build/mocha/arm-eabi-6.x/bin/arm-eabi-
export KBUILD_BUILD_HOST="Sup-Mint"
export KBUILD_BUILD_USER="Yaro330"
make tegra12_android_defconfig
make -j4 ARCH=arm CROSS_COMPILE=/home/$USER/build/mocha/arm-eabi-6.x/bin/arm-eabi- zImage
cp $KERNEL_DIR/arch/arm/boot/zImage $KERNEL_DIR/build/zImage

