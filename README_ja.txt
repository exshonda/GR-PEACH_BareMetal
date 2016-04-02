○フォルダ構成


○インストール

●TrueStudioのインストール

本パッケージは Atollic TrueStudio をサポートしている．
TrueStudio は，Free版(サイズ制限なし)を以下からダウンロード可能である．

http://timor.atollic.com/truestudio/


●ドライバのインストール

GR-PEACHのPG側のUSBポート(リセットボタン側)とPCを接続する．

シリアルポートして認識されない場合は以下のドライバをインストールする．

https://developer.mbed.org/handbook/Windows-serial-configuration


○TrueStudioのプロジェクトの作り方

●ワークスペースの作成

PC毎に1回実施すればよい．

1.TrueStudioを起動する．

2.ワークスペースの場所を聞かれるので，./truestudio を絶対パスで指定す
  る．
  
3.デバッガの設定を行う  

左のツリーから -> "実行/デバッグ" -> "組込みC/C++アプリケーション" -> 
"デバッグハードウエア" -> "OpenOCD" を選択する．

"サーバ"に.\tools\0.10.0-201601101000-dev\bin\openocd.exe を絶対パス指
定する．"ディレクトリ"に.\tools\0.10.0-201601101000-dev\を絶対パス指定
する．

"ウェイト時間"は2程度に設定する．

●既存プロジェクトのオープン

各プロジェクトのフォルダにある .cproject をクリックする．TrueStudioが
起動するので，上記で作成したワークスペースを指定する．

●新規のプロジェクトの作成方法

1.ファイルのコピー
他のプロジェクトのMakefileと.c/.hファイルをコピーする．TrueStudioのフ
ァイル(.で始まるファイル)はコピーしないこと．

2.プロジェクトの作成
2.1 メニュー "ファイル"->"インポート"を選択．
2.2 Existing Code as Makefile Project を選択して次へ．
2.3 プロジェクト名を指定．既存のコード場所で上記のフォルダを指定．
    インデクサー設定に対するtoolchainに"Atolli ARM Tools"を選択．
    終了をクリック
2.4 エクスプローラからbaseフォルダをSHIFTボタンを押しながらプロジェク
    トにドロップする．"ファイルおよびフォルダーにリンク"を選択して"OK"
    を押す．
2.5 メニュー"プロジェクト"->"C/C++ビルド"の設定を選択
    設定 -> "Target Settings" を次のように設定する．
     マイクロコントローラファミリ -> Generic Cortex-A
     マイクロコントローラ         -> Cortex-A9
    Tool chain エディターを選択して
     現在のビルダーに"Gnu Makeビルダー"を選択
    C/C++ビルドを選択
     Makefileの生成の"自動的にMkefileを生成"のチェックボックスを外す． 

3.ビルド
3.1 メニュー "プロジェクト"->"プロジェクトのビルド"を選択してビルド．

4.デバッグ
4.1 メニュー "実行"->"デバッグ"を選択．
4.3 名前にプロジェクト名を選択
4.4 "デバッガ"タブを選択．デバッグプローブに"OpenOCD"を選択．
    GDBサーバーの起動パラメータに以下を貼り付け．    
    -f ./scripts/interface/cmsis-dap.cfg -f ./scripts/target/renesas_rza1h_swd.cfg 
4.5 "スタートアップスクリプト"のタブを選択．
    "ターゲットソフトウェア初期化スクリプト" のタブルに以下の内容を貼
     り付け．
    
----------------------------------------------    
monitor init
monitor reset
monitor halt

# Release L2 cache standby
set {unsigned int}0x3fffff80=0x00000001

# Allow write access for RAM     
set {unsigned char}0xfcfe0400=0x3f
x/b 0xfcfe0400
set {unsigned char}0xfcfe0404=0x3f
x/b 0xfcfe0404
set {unsigned char}0xfcfe0408=0x0f
x/b 0xfcfe0408

load
----------------------------------------------    
