# 概要
最新論文を追実装してみようという試み

# 動作確認環境
El capitan OS X 10.11.5
openFrameworks v0.9.3

# 依存関係
hdf5の読み込みのためにstatismoを利用しています.
これらは既にPCの/usr/localにインストールされていることを前提にプロジェクトが組まれています.

インストール方法は[こちら](./document/Install.md)

# 使い方とか
現状はopenFrameworksのプロジェクトなので、このリポジトリを丸ごとapps/myApps以下においてください。

サブモジュールを更新する必要があるので、以下のコマンドを打っておいてください。

    git submodule update --init --recursive

# 使っている外部ライブラリ
 * [ofTimeMeasurements](https://github.com/armadillu/ofxTimeMeasurements)
 * [Eigen](http://eigen.tuxfamily.org/index.php)

# ドキュメント
 * [Face2Face日本語要約](./document/Face2Face-jp.md)

# 参考(お世話になったサイトとか)
元論文
[Face2Face](http://www.graphics.stanford.edu/~niessner/thies2016face.html)

gitignoreの自動生成ツール
[gitignore.io](https://www.gitignore.io/)

# ライセンス
このソフトウェアは、[Mozilla Public License Version 2.0](http://mozilla.org/MPL/2.0/)ライセンスで公開しています。

