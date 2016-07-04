# Face2Face掲示板

### Q1. ビデオから生成されたターゲットシェイプは，Expression情報(ブレンドシェイプ)を持っている？

７６個のブレンドシェイプを持っていて，７６個のうち４６個はChen CaoのFaceWarehouseからNon-rigid ICPをしたもので、残りはアーティストが作ったもの

####  Q1.1  Non Rigid ICPにはソースとターゲットは何を入力しているのか

ソースは、Caoのfacewarehouse、ターゲットはBlanzのMorphableモデルのメッシュ．
	Non-Rigid ICPでMorphable modelをFacewarehouseの形状にフィットさせるので頂点数等はmorphableモデルと一緒になる．

#### Q1.2 ソースとして使用しているFacewarehouseには何人もの顔の形状のデータベースがあるが，その中からどうやって一つに決めてるのか(作っているのか)？
不明->未解決問題

### Q2. 3章で書かれている80とか76とかいう数値は，3Dの顔データベースにPCAをかけて得た上位80個の主成分(Expressionの場合は76個)っていう理解であってるか？
あっている
> 彼らのモデルはBlanz and VetterのMorphable Modelを使っていて、albedoとidentityのdeformationに関しては２００人からサンプルされたデータをPCAかけたものを使っています。なので８０という数である必要はないですが、残りの１２０個はかなり小さいコントリビューションなので、無視しても最適化にそこまで影響がないというのが80個のパラメータを使っている理由です。Expressionに関してはSIGAの論文にある通り、PCAはかけていませんね。それはCVPRの方も同様だと思います。

#### Q2.1 200人からサンプルされたalbedoとidentityはFacewarehouseのデータを使ってる？

違う。これはmorphable modelを使用している．

###	Q2.2 morphable modelに画像データも入っているのか

Basel Face Modelに含まれてるっぽい
http://faces.cs.unibas.ch/bfm/main.php?nav=1-1-0&id=details

Viewerはこれが使えそう->oFに移植予定
https://github.com/statismo/statismo

### Q3. なぜ、ブレンドシェイプウェイトがあるにもかかわらず、Deformation Transferをわざわざ使っているのか

> 補足として、「なぜ、ブレンドシェイプウェイトがあるにもかかわらず、Deformation Transferをわざわざ使っているのか」という疑問が湧いてくると思うんですが、彼らのShapeモデルは下のようにidentityとexpressionを線形和で足し合わせたものになっています。E_idとE_expに十分な自由度があれば、大概の顔の形状は再現できますが（これがFWの４６個のブレンドシェイプに加えて、アーティストメッシュを３０個加えた理由だと思います）、identityとexpressionが完全に独立しているので、ある特定の人の表情を再現するために使われたδが異なる人の同じ表情を再現するという保証はありません。

>  これに対して、Haoの2013年の論文や、Sofienの2013年の論文は、IncrementalにExpression Spaceをupdateしていくという方法で、より個人性を反映させたExpressionを計算しています。他のbranchでいうとCaoは、ExpressionとIdentityに相関を持たせるためにテンソルを使ったBi-linearモデルで表情を計算しています。つまり、Reviewerからの「同じδが他の人で同じ表情を再現する根拠はどこにあるんだ」というツッコミを防ぐために、ターゲットのgeometryを考慮したDeformation Transferを用いた方法を使っているんだと考えられます。

#### Q3.1 結局，Deformation Transferを使うことによる効果は顕著に表れるのか

> 実用という観点からいうと「そんなに大差ない」というのが正直なところで、もちろんside-by-sideで見ればperson-specificなexpressionの方がより正確に表情をキャプチャーしていることが確認できますが、単純に生成結果を見るだけならばほとんど区別できません（この点はJP lewisやHaoも同様の見解です）。なので、何か動くデモを作りたいだけなら、ブレンドシェイプの係数δをtransferするだけでも問題ないと思います。

### Q4. 個人のidentityを作るときはMorphable Model、expressionを作るときはFacewarehouseを使ってるという認識は正しいか？

正しい

### Q5. 最初にターゲットアクターのidentity作るって言ってるけどソースはどうするのか?リアルタイムでいいのか？{P.1, 3段落 In our method, ~}

事前のInitializeフェーズでソースとターゲット両者のIdentityを作成して、ランタイムフェーズではIdentityのうちのshapeとalbedoはシーケンス中で不変であると仮定して、expressionのパラメータだけ解いている。

今回の論文に当てはめると、

> In our method, we first reconstruct the shape identity of the target actor using a new global non-rigid model- based bundling approach based on a prerecorded training sequence. As this preprocess is performed globally on a set of training frames, we can resolve geometric ambiguities common to monocular reconstruction. At runtime, we track both the expressions of the source and target actor’s video by a dense analysis-by-synthesis approach based on a sta- tistical facial prior.

と言っていて、一見ソース側のshapeとalbedoのことはnon-rigid model- based bundlingすると書いていないのでリアルタイムで解いているように読めるが、ここ以外の文脈やFigure 1を見るとIdentityをリアルタイムで解いているというふうには見えない。

そして、6章の冒頭は、

> To estimate the identity of the actors in the heavily under- constrained scenario of monocular reconstruction, we intro- duce a non-rigid model-based bundling approach.

となっていて、of the actorsと言っていることからもソースのアクターもここに含まれていると思われる。
ビデオの絵も640x400くらいの解像度に見えないし、多分ターゲットと同様の方法でソースも作っているで良い気がする。
あと、7章の転写のところも、

> Assuming source identity αS and target identity αT fixed, transfer takes as input the neutral δSN , deformed source δS , and the neutral target δTN expression.
>
てな感じでshapeが固定という仮定を用いているので、ソースも事前作成してランタイムでは固定ってことでよいのだと思う。

モーファブルモデルで事前作成してるってことで一旦done

### Q6. ターゲットの表情トラッキングは事前じゃないのか？

論文の中盤とかFig1では事前トラッキングしているように見えるけど、最初の説明のところではboth source and target的な感じでランタイムにトラッキングするといっている。
どうなんだろうか

どっちみちビデオ映像だから事前で問題ないはずなのに無駄にランタイムトラッキングしてるみたい。ただ、事前計算として（おそらくトラッキング等を使って）ターゲットの形状構築を行うと書いてあって、じゃあなぜソースの形状構築だけリアルタイムで行われているのかは謎に包まれてる。

>we first reconstruct the shape identity of the target actor using a new global non-rigid model based
bundling approach based on a prerecorded training sequence. As this preprocess is performed globally on a set of training frames, we can resolve geometric ambiguities common to monocular reconstruction

これはリアルタイムトラッキングしている。
ランタイムで使っているものとIdentity作成は別シーケンスであると捉えてよいと思う。
サプリメントドキュメントを読むと、Identityの作成には6フレームくらいしか使っていないっぽい(ウル覚え。たしか)

結局これテクスチャが出力になるので、パラメータを全てキャッシュすると膨大なデータ量になる。
当然メモリに乗らないし、読み込む方がむしろ大変なのではないだろうか。

※ここで6フレームって言っているのは使用した720pのHD画像6フレーム分という意味。計算には20秒くらいかかっている。
