Docker環境+VSCode+QEMU対応版です。<br>
1.Docker環境としてalvstakahashi/stm32toolchain:1　でビルド可能になります。<br>
また.devcontainerを追加していますので、VSCODEでのリモートコンテナ利用が可能です。<br>
2.QEMU対応を行いました。　利用するシステムタイマーを変更して対応しました。make runqで実行可能です。<br>
2021.12.20


# RPI-SHRINK-SSP-FULL
BareMetalで遊ぶRaspberryPi[達人出版」の環境にて、SSPのシュリンク版を移植したものです。

2017.08.21 
branch forRPI3-JTAG を更新　バグ修正
1.カーネル動作全般をSVCモード動作に戻した
2.割り込みのレジスタ退避を追加



2017.8.18 
branch forRPI3-JTAG を更新
ラジコン制御用にタイマー周期を10usにした。
その他バグ修正
1.割り込み時の割り込み許可禁止箇所の見直し
2.割り込み用スタックエリアの使用。基本カーネルはIRQモード、タスクはSVCモードとした
3.GCCのattribute(interrupt)ではlrが保存されないので、保存するようにした。
　別紙　lrレジスタの保存について.txt に経緯を記載




2017.8.16
branch forRPI3-JTAG を作成
待ちあり版、RPI3専用、aarch32仕様、JTAG対応
GCCのattribute(interrupt)を使いさらにアセンブリ言語の部分を
縮小した。
現在startup部分のみ
ベクターテーブルおよび割り込みはCで記載


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
