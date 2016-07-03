コンピュータアルゴリズムについて

アルゴリズムを改良したので、コンピュータも積極的に役を狙ってきます。
そのため前のアルゴリズムよりは若干強くなったようです。

概要は以下の通りです。


	コンピュータは、手牌を一枚づつ 点数をつけていって、
	最も点数の低いものを捨てます。

	点数は、次のものの総合点になります。

	BASE   : 牌の組み合わせ
	RANDOM : 乱数
	DORA   : ドラ牌
	ZONE   : 1-9 の数字の片寄り方
	SUJI   : 筋牌の片寄り方
	YOMI   : 現物牌/危険牌のチェック

        それぞれの 重みを pf によって 変更することが可能です。
	BASE は固定で、1000 になっています。

	テンパっているときリーチするかどうかの判断は、
	リーチ確率によって決めます。
	リーチ確率は、山の残りの枚数が少なくなれば、だんだん小さく
	なるようになっています。

	リーチ確率は、 pf によって変更可能です。


	また、次の ポリシー が成立した場合 BASE 評価アルゴリズムを変えます。
	これらも pf によって 成立条件を変更できます。

	CHITOI  : チートイツねらい 
	KOKUSHI : 国士ねらい
	TOITOI  : トイトイねらい
	TANYAO  : タンヤオねらい
	COLOR   : 染めねらい
	YAKU    : 役牌ねらい

	なくかどうかの判断については、役牌 の 有無 や シャンテン数
	によって判断します。

	また、ポリシーが成立している場合は、ポリシーによって
	なくかどうかを判断します。


パラメータについて、

	パラメータ名/デフォルト値 は、以下の通りです。

		chitoi		700
		kokushi		700
		toitoi		700
		tanyao		700
		color		700
		yaku 	 	700

		random		 50
		dora		100
		zone		300
		suji		100
		yomi 	 	500

		reach 	 	700
		kan 	 	300
		pon 	 	300
		tie 	 	300

	注)
	color の意味が古いバージョンと違います。

pf ファイルの例
--- ここから
#!/bin/sh
# netmaj personal file
param: chitoi = 700
param: kokushi = 700
param: toitoi = 900
param: tanyao = 900
param: color = 900
param: yaku = 700

param: random = 50
param: dora = 200
param: zone = 500
param: suji = 200
param: yomi = 700

param: kan = 400
param: pon = 400
param: tie = 400
# result format
#      NO  PNT  TN PN RN NN    TUMO        RON       FURI
-- ここまで


おまけ
auto の pf ファイルに 
	param: noisy = 1
を加えるのは...やめましょう。


