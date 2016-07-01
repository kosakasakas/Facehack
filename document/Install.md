# Macの場合

homebrewで依存関係を丸ごとインストールします.

    brew tap homebrew/science
    brew install statismo 

statismo内でPIがduplicateするので以下で回避した。

    #ifndef PI
    const double PI =   3.14159265358979323846;
    #endif

以上
