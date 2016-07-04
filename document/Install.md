# Macの場合

homebrewで依存関係を丸ごとインストールします.

    brew tap homebrew/science
    brew install statismo 

statismo内でPIがduplicateするので以下で回避した。

    #ifndef PI
    const double PI =   3.14159265358979323846;
    #endif

## Permission deniedになる場合の対処

　　$ sudo chown -R `whoami` /usr/local
　　$ cd /usr/local
　　$ git reset --hard origin/master

以上
