He Is Not a Dog But a Cat
version 0.0.1   12/25/2018
(C) 2018 Kazuma Murata 



1. ソフトの概要
Handwritten Clock は手書きの文字盤と針で作られたアナログ時計です.
ドラッグにより描画色,背景色の変更も可能です.

2. 内容物
内容物は以下の通りです.
 - icon.rc     アイコン設定用のファイルです.
 - LICENSE     ライセンスが記載されたファイルです.
 - Makefile    Makeを実行するのに使うMakefileです.
 - README.txt  このファイルです.
 - resource    リソースが入っているディレクトリです.
 - src         ソースコードが入っているディレクトリです.

3. ビルド方法
glutとglpngがインストールされたcygwin上でこのファイルがあるディレクトリに移動し,debug情報を利用したいときは
> make debug
を実行してください.
それ以外の時は
> make release
を実行してください.
j15435.exeというファイルが作成されます.
作成されたファイルを削除したいときには
> make clean
を実行して下さい.

また,環境に応じて,必要なときはMakefileを変更してください.

4. 使い方
cygwin1.dllとglut32.dllがパスに通った環境でj15435.exeを実行してください.
新しいウインドウが開かれ,時計が表示されます.
ウインドウ上で左,右マウスボタンによるドラッグを行うことにより,描画色,背景色の変更を行うことができます.
ウインドウの左上を原点として,ウインドウ右端がxの最大値,ウインドウ下端がyの最大値としたときの,
マウスカーソルの座標と変更後の色(RGBA)との座標との対応を以下に示します.

R : マウスボタンを押したときのマウスカーソルのx座標
G : マウスボタンを押したときのマウスカーソルのy座標
B : マウスボタンを離したときのマウスカーソルのx座標
A : マウスボタンを離したときのマウスカーソルのy座標

プログラムを終了するときには,キーボードのq, ESCのいずれかのキーを押すか,マウスのミドルボタンをクリックしてください.
ウインドウの閉じるボタンから終了しようとすると,ウインドウは閉じられますが,プログラムの実行が止まりません.

5. 動作環境
以下の環境において動作確認を行っています.

CPU    : Intel(R) Core(TM) i5-8400 CPU @ 2.80GHz
GPU    : NVIDIA GeForce GTX 970
OS     : Micorsoft Windows 10 Education ver.10.0.17134 build 17134
Cygwin : CYGWIN_NT-10.0-WOW 2.11.2-1 i686 Cygwin
GLUT   : GLUT 3.7
GCC    : 7.3.0

CPU    : Intel(R) Core(TM) i5-3230M CPU @ 2.60GHz
GPU    : Intel(R) HD Graphics 4000
OS     : Micorsoft Windows 10 Pro ver.10.0.17763 build 17763
Cygwin : CYGWIN_NT-10.0-WOW 2.11.2-1 i686 Cygwin
GLUT   : GLUT 3.7
GCC    : 7.3.0

6.ライセンス
本ソフトウェアはのライセンスはMozilla Public License, v. 2.0.に従います.
詳しくは同梱のLICENSEを参照してください.
