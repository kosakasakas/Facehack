# boostのインストール


# HDF5のインストール

* [公式ページ](https://www.hdfgroup.org/)から最新のソースコードを落としてくる。自分の場合は2.8.17でした.
* フォルダを解凍して以下のコマンドを実行

    cd hdf5-1.8.17
    ./configure --prefix=$HOME/local --enable-cxx
    make
    make install
