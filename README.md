# 1. Introduction
我々の手法は最新の手法と似たアプローチを取るが、単眼からの顔の復元をリアルタイムに行えるという点にコントリビューションがある。
[5,11,13]はリアルタイムではない。我々はリアルタイムにRGBだけで表情転写するのが目的。

> [5] C. Bregler, M. Covell, and M. Slaney. Video rewrite: Driving visual speech with audio. In Proc. SIGGRAPH, pages 353–360. ACM Press/Addison-Wesley Publishing Co., 1997.

> [11] K. Dale, K. Sunkavalli, M. K. Johnson, D. Vlasic, W. Matusik, and H. Pfister. Video face replacement. ACM TOG, 30(6):130, 2011. Proceeding Series, pages 281–288. ACM, 2006.

> [13] P. Garrido, L. Valgaerts, O. Rehmsen, T. Thormaehlen, P. Perez, and C. Theobalt. Automatic face reenactment. In Proc. CVPR, 2014.

我々の手法は単眼ビデオならなんでもいける。例えば、Youtubeからダウンロードした昔の映像でも大丈夫。修正が入っていることが気づかないレベルで、古いビデオを最新の形式に変換することなんかもできる。フォトリアルなビデオからの顔の復元はいろんなアプリケーションに応用が考えられる。例えば。。。。。

本手法ではまず、ターゲットの基準形状?(the shape identity of target actor)を事前収録されたシーケンスから、新しいグローバルな非剛体な手法で作成する。この事前処理はトレーニングフレーム全体でグローバルに実施される。これにより、単眼だと一般的に形状が曖昧になってしまう点がカバーされる。ランタイムでは、静的な顔の事前分布?(statistical facial prior)に基づく顔の合成分析?(analysis-by-synthesis)により、ソースとターゲットの表情をトラッキングする。これらの表情の転写精度は、最新のデプスを使った研究と比べても見劣らないことも示している。また、リアルタイムに処理を行うために、低次元の表情空間上で直接デフォメーショントランスファー[27]を効果的に行う画期的な手法を提案している。

> [27] R. W. Sumner and J. Popovic. Deformation transfer for triangle meshes. ACM TOG, 23(3):399–405, 2004.

最後の画像合成時には、背景画像から推定したライティングを施し、ターゲットの表情付きの顔を再レンダリングして、背景と混ぜている。
最後に、新しい画像ベースの写実的な口内合成手法も紹介する。この手法は、オフラインのサンプルシーケンスから一番マッチする口内形状を回収してくる方法だ。ターゲットの口内形状を維持するのは重要なはずだが、既存の手法はソースのマウス形状をコピーしてきたり[30,11]、代わりの歯を生成してレンダリングしたり[14, 29]、矛盾のある結果を生むものしかない。
図1は我々の手法の全体像である。

![fig1](file:///Users/takahiro.kosaka/Downloads/fig1.png "fig1")

本手法は初のRGBだけでリアルタイムに表情転写できる手法である。コントリビューションはまとめると、以下の点である。

 * グロ-バルな非剛体モデルベースビルディングである
 * 正確なトラッキング、見た目、ライト推定を自由なliveRGBビデオで実現可能
 * 個人に依存する表情の転写を副次空間で行っている
 * 画期的な口内合成の手法を提案している

# 2. Rerated Works
## Offline RGB Performance Capture
※意味ありそうな関連研究の部分しか取り上げません。
Shiらは選択されたキーフレーム全体のエネルギー最適化により良い結果を得ている。我々の、基準顔生成のためのモデルベースビルディング関数はこのアプローチに似ている。しかし、我々はGPUで並列処理することができるロバストで密なグローバルフォトメトリップアライメントを用いている。

> [26] F. Shi, H.-T. Wu, X. Tong, and J. Chai. Automatic acquisition of high-fidelity facial performances using monocular videos. ACM TOG, 33(6):222, 2014.

## Online RGB-D Performance Capture
いろいろあるけど、デプス使っている時点でダメだよね。

## Online RGB Performance Capture
いろいろあるけど、高画質じゃなきゃだめだからあかん。

## Offline Reenactment
Garridoらは自動で完全にイメージベースドなアプローチで全体の顔の入れ替えを行っている。ただし、これは単に自分の顔の入れ替えでしかなくて、我々はソースとターゲットが異なっていても大丈夫。
川井らは口内画像の再合成を行い、歯と舌の2Dアニメ-ションを生成している。彼らは口のフロント部分しか生成できないため、我々のようなヘッドモーションに即したリアルなレンダリング結果は描けない。

> [13] P. Garrido, L. Valgaerts, O. Rehmsen, T. Thormaehlen, P. Perez, and C. Theobalt. Automatic face reenactment. In Proc. CVPR, 2014.

> [18] M. Kawai, T. Iwao, D. Mima, A. Maejima, and S. Morishima. Data-driven speech animation synthesis focusing on realistic inside of the mouth. Journal of Information Processing, 22(2):401–409, 2014.

## Online Reenactment
デプス使っているのは何個かある。
この論文は、1) 最初のRGB-Onlyのリアルタイム手法である 2) 口内もターゲット側から生成するので代理の歯のモデルなどもいらない

# 3. Synthesis of Facial Imagery
我々はマルチレイヤPCAモデル[3,1,9]を使う。最初の二事前は顔のアイデンティティを示している。すなわち、ジオメトリと、肌の反射率のこと。三次元目は顔の表情をあらわしている。ゆえに、我々は顔というものを以下のようにパラメタライズする:

TBD..

# 7. Expression Transfer
各アクターの表情の個性を保ちながらソースからターゲットのアクターへと表情を転写するために，sub-space deformation transferの技術を提案する．

Sumner[27]との違い

Sumnerらのdeformation transfer energyのアプローチとは異なり，expression blendshapesによって間引かれた空間の中で直接処理をする．これは，擬似逆行列?(pseudo-inverse of the system matrix)の事前計算だけでなく，高速な転写速度を実現する最適化問題の次元削減も可能にする．ソースとターゲットの個性?(identity)を固定されたものと仮定し，neutral, deformed source, そして，transfer sourceを入力とする．また，事前に削減された部分空間内でのtransferred facial expressionが出力となる．

手順

Sumnerの手法と同様，まずソースの三角形を初期形状から変形形状へ変形させるための，ソース形状の初期変形勾配(deformation gradients)を計算する．その変形先は，線形最小二乗問題(linear least-squares problem)を解くことにより変形前の状態に基づいて見つけることができる？ 三角形に含まれる頂点のインデックス3つを用いて，VとV^を計算するが，その際未知のターゲット変形が最小化される(式7).この問題はcanonical least-squares形式に書き換えることができる(式8)．

その行列Aは一定で，表現部分空間(expression sub-space)に投影されるテンプレートメッシュのエッジ情報を含んでいる．初期表情内でのターゲットのエッジ情報は，right-hand side bを含んでいる．deformed source(delta^s)付きのbは，各更新入力フレームでGPU計算される．二乗エネルギー(quadratic energy)の最小化は，corresponding normal equationを解くことによって計算される．system matrixは一定のため，我々は，そのPseudo InverseをSingular Value Decomposition(SVD)を用いて事前計算することができる．その後，76*76の線形システムがリアルタイムで解かれる．それらしい形状や滑らかさを保証するようブレンドシェイプモデルを制限しているため，[27,4]のような追加のスムーズ化処理を必要としない．
