# RPI-SHRINK-SSP-FULL
BareMetalで遊ぶRaspberryPi[達人出版」の環境にて、SSPのシュリンク版を移植したものです。

2017.7.22
raspberry-pi3 対応
・makefile.rpi3 を利用すればRPI3用バイナリができます。
・pi1 pi2 は従来のmakefileを利用
・pi3 の場合は、config.txtに以下2行のみ記載してください
disable_commandline_tags=1
kernel_old=1
 

20150720
1.WAIT-SSP(dly_tskのあるカーネル)にしました。

ssp-rpi-20150618-RPI2 リリースノート

raspberry pi2 タイプBに対応しました。
RPI1のタイプBの場合もmakeのオプションで指定できるようにしました。


ビルド方法は　make で機種を指定ください

RPITYPE=RPI2		//raspberryPi2  
RPITYPE=RPIB-PLUS	//raspberryPi B+  
(未指定)			//raspberryPi B  

(例）
$ make RPITYPE=RPI2

ビルド環境
以下URLのubuntu32bit環境でビルド可能です。
http://dev.toppers.jp/trac_user/contrib/browser/CROS-TOOL-VMWARE




以下　過去のリリースノート------------------------------------

20150611版リリースノート

20150602版は正しく動かないので、削除しました。

以下URLのubuntu32bit環境でビルド可能です。
http://dev.toppers.jp/trac_user/contrib/browser/CROS-TOOL-VMWARE

Raspberry pi TYPE B+　対応にしました。

TYPE Bで確認するには、LEDの番号のdefineを変えてください。　main.c
