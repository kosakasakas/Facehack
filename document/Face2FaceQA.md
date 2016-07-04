# Face2Face掲示板

### Q1. ビデオから生成されたターゲットシェイプは，Expression情報(ブレンドシェイプ)を持っている？

７６個のブレンドシェイプを持っていて，７６個のうち４６個はChen CaoのFaceWarehouseを基にして作られた顔形状，残り30個はアーティストが作ったもの．
ベースとなっている顔モデルはMorphable Modelで，後頭部のないお面データとなっている．

####  Q1.1  Expressionの作り方

~~Non-Rigid ICPでMorphable modelをFacewarehouseの形状にフィットさせるので頂点数等はmorphableモデルと一緒になる．~~

Morphable ModelのIdentityが定義されたモデルに対し，Deformation Transferを使って，表情を付与．Deformation Transferのソースは、Morphable Modelのメッシュで，ターゲットは２つの方法を組み合わせて作っている．一つは，アーティストによる手作業．もう一つは，Caoの手法と同様に，スキャンした形状にフィットするように変形させるNon-rigid registrationを用いることで作成されている．

Chapter 4.1 [SIGGRAPH ASIA 2015]
> Facial expressions are added to the identity model by transferring the displacement fields of two existing blend shape rigs by means of deformation transfer [Sumner and Popovic 2004]. The used blend shapes have been created manually [Alexander et al. 2009] or by non-rigid registration to captured scans [Cao et al. 2014b].

今回の実装では，Caoの手法であるNon-rigid ICPは用いず，Caoの手法で作られた顔データベース「Facewarehouse」を用いて，ある人物のExpression 46個の表情モデルをDeformation Transferのターゲットモデルとする．

> キーシェイプはPCAのミーンシェイプに対して作る

> それって、Face Model Fittingするときにうまくある？その人の個人の顔とその顔でのexpressionがあるほうが、評価関数のエラーが小さくなるのでは？うまくいく？

> 彼らの導入してる顔モデルは表情空間と顔のジオメトリ空間が完全に分離してる
だからブレンドシェイプとPCAが混在できるし、そうじゃないとソースもターゲットも同じ顔モデルで扱えない．表情トランスファーの部分の仮定とかが成り立たない．つまり、表情に個性はないってことだよ

> なるほどたしかに評価してるのは表情の差分だけだから、mean shapeのexpressionがあるだけでいいのか

Expressionに関しては，無表情との差分が重要であるため，ソースもターゲットもIndividual Expressionを作る必要はないという説が濃厚．

#### Q1.2 ターゲットとして使用しているFacewarehouseには何人もの顔の形状のデータベースがあるが，その中からどうやって一つに決めてるのか(作っているのか)？

不明->未解決問題

どの人物のExpressionを選んでも大きな差はなさそうというのが現段階での考察　　
また，Face2Faceの論文では，個人の表情変化形状に関する信ぴょう性は高くないのが所感．[関連] Q3

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

本実装では，Expression Transferは行わずに，ソースで求められたブレンドシェイプをターゲットに直接適用する予定．

### Q4. 個人のidentityを作るときはMorphable Model、expressionを作るときはFacewarehouseを使ってるという認識は正しいか？

正しい

### Q5. 最初にターゲットアクターのidentity作るって言ってるけどソースはどうするのか?リアルタイムでいいのか？{P.1, 3段落 In our method, ~}

事前のInitializeフェーズでソースとターゲット両者のIdentityを作成して、ランタイムフェーズではIdentityのうちのshapeとalbedoはシーケンス中で不変であると仮定して、expressionのパラメータだけ解いている。

今回の論文に当てはめると、

> In our method, we first reconstruct the shape identity of the target actor using a new global non-rigid model- based bundling approach based on a prerecorded training sequence. As this preprocess is performed globally on a set of training frames, we can resolve geometric ambiguities common to monocular reconstruction. At runtime, we track both the expressions of the source and target actor’s video by a dense analysis-by-synthesis approach based on a sta- tistical facial prior.

と言っていて、一見ソース側のshapeとalbedoのことはnon-rigid model- based bundlingすると書いていないのでリアルタイムで解いているように読めるが、ここ以外の文脈やFigure 1を見るとIdentityをリアルタイムで解いているというふうには見えない。

そして、6章の冒頭は、

> To estimate the identity of the actors in the heavily under- constrained scenario of monocular reconstruction, we introduce a non-rigid model-based bundling approach.

となっていて、of the actorsと言っていることからもソースのアクターもここに含まれていると思われる。
ビデオの絵も640x400くらいの解像度に見えないし、多分ターゲットと同様の方法でソースも作っているで良い気がする。
あと、7章の転写のところも、

> Assuming source identity αS and target identity αT fixed, transfer takes as input the neutral δSN , deformed source δS , and the neutral target δTN expression.

てな感じでshapeが固定という仮定を用いているので、ソースも事前作成してランタイムでは固定ってことでよいのだと思う。

モーファブルモデルで事前作成してるってことで一旦done．

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

### Q7. IdentityとAlbedoは事前処理で求めるだけでよいのか．残りはFix?

事前処理として，IdentityとAlbedoは一度推定し，毎フレームの最適化では固定とする．

### Q8. ターゲットも毎フレームExpressionの係数を最適化によって求めているのか？

+ 求められている場合
	+ Expression Transferによって差分を更に付与
	+ Expression Transferによって表情が上書き
		+ 一旦すべてを係数ゼロにして無表情に戻してから加えるのと同じ？


+ 求められていない場合
	+ Expression Transferによって差分を更に付与
