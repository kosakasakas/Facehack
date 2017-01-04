## Status
Facehack is in development. So, it is not able to capture your facial expressions yet.  
I have been developed the facehack as a hobby, so please forgive my slow update.  
And I would be very happy if you could help me for developing the facehack.  
Please feel free to ask me about the facehack.  
e-mail: tkosaka15@gmail.com  
Thanks,
  
facehackは現在開発中です.なので表情の反映はまだできません.  
facehackの開発は趣味として行っていますので、更新が遅れることをどうかご了承ください.  
そして、facehackの開発に協力してくれる方がいたらとても嬉しいです.  
facehackに関してお気軽にご連絡ください.  
e-mail: tkosaka15@gmail.com  
よろしくお願いします.  

# 概要
最新論文を追実装してみようという試み

# 動作確認環境
El capitan OS X 10.11.5
openFrameworks v0.9.3

# 依存関係
hdf5の読み込みのために[statismo](https://github.com/statismo/statismo)を利用しています.
これらは既にPCの/usr/localにインストールされていることを前提にプロジェクトが組まれています.

インストール方法は[こちら](./document/Install.md)

# 使い方とか
現状はopenFrameworksのプロジェクトなので、このリポジトリを丸ごとapps/myApps以下においてください。

サブモジュールを更新する必要があるので、以下のコマンドを打っておいてください。

    git submodule update --init --recursive

# 使っている外部ライブラリ
 * [ofTimeMeasurements](https://github.com/armadillu/ofxTimeMeasurements)
 * [Eigen](http://eigen.tuxfamily.org/index.php)
 * [statismo](https://github.com/statismo/statismo)

# ドキュメント
 * [Face2Face日本語要約](./document/Face2Face-jp.md)
 * [Face2Face掲示板](./document/Face2FaceQA.md)  

# 参考
元論文
[Face2Face](http://www.graphics.stanford.edu/~niessner/thies2016face.html)

# ライセンス
このソフトウェアは、[Mozilla Public License Version 2.0](http://mozilla.org/MPL/2.0/)ライセンスで公開しています。

