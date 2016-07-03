server.pf につぎのパラメータを記述することにより、
若干ルールを変更できるようになりました。

なお、メニューのルールを選ぶことにより、ルールの確認ができます。


I) 役に関するもの

[記述]
	param: nopintumo = 1
[意味]
	ピンヅモなし: つもったときは、ピンフがつかない

[記述]
	param: noakadora = 1
[意味]
	赤ドラなし: 赤ドラは、ドラ扱いにしない

[記述]
	param: nonakitan = 1
[意味]
	なきタンなし: ないた時はタンヤオがつかない

[記述]
	param: nokanura = 1
[意味]
	カンウラなし: ウラドラは１枚のみ


 II) ゲーム運びに関するもの

[記述]
	param: nofly = 1
[意味]
	とびなし: 最後までゲームをする (あがりやめもなし/箱下計算もあり)

[記述]
	param: under = 1
[意味]
	箱下計算あり: 0 点以下の分を 0 点扱いにせず計算する

[記述]
	param: startpoint = 数字
		ただし 25000 <= 数字 <= 30000
[意味]
	ゲーム開始時の持ち点

[記述]
	param: atamahane = 1
[意味]
	頭ハネあり：ダブロンのときは、頭ハネの１人しかあがれない。

[記述]
	param: nosancyaho = 1
[意味]
	三家和なし（頭ハネなしの場合はトリロンあり

[記述]
	param: 77mangan = 1
[意味]
	子の7700/親の11600は満貫扱い

[記述]
	param: nokuikae = 1
[意味]
	喰い替えなし: 喰ったとき、喰った牌と喰い替えになる牌は切れない

[記述]
	param: pao = 1
[意味]
	包（パオ）あり: 大三元、大四喜を確定させると責任払い


 III) 御祝儀(チップ)に関するもの


[記述]
	param: yakuman-chip = 数字
		ただし 0 <= 数字 <= 8
[意味]
	(つもったときの)役満御祝儀の枚数
	振り込みは、その５割増し。
	
[記述]
	param: ippatu-chip = 0
[意味]
	一発役満御祝儀なし

[記述]
	param: uradora-chip = 0
[意味]
	ウラドラ御祝儀なし

[記述]
	param: akadora-chip = 0
[意味]
	赤ドラ御祝儀なし

[記述]
	param: nonakiaka = 1
[意味]
	赤ドラは面前のみ: ないた時は赤ドラがつかない

[記述]
	param: noallstars = 1
[意味]
	オールスターなし: 赤ドラを３枚集めたときの御祝儀(2枚) をやめる


 IV) ウマ(順位点) に関するもの

[記述]
	param: uma = none
[意味]
	ウマなし

[記述]
	param: uma = 5-10
[意味]
	ゴットー
	トップが +10 point ２位 +5 point ３位 -5 point ４位 -10 point

[記述]
	param: uma = 10-20
[意味]
	ワンツー
	トップが +20 point ２位 +10 point ３位 -10 point ４位 -20 point
[記述]
	param: uma = 10-30
[意味]
	ワンスリー
	トップが +30 point ２位 +10 point ３位 -10 point ４位 -30 point

[記述]
	param: uma = 7-5-3
[意味]
	シチゴサン

	一人浮きのとき
	  トップが +15 point ２位 -3 point ３位 -5 point ４位 -7 point
	二人浮きのとき
	  トップが +10 point ２位 +5 point ３位 -5 point ４位 -10 point
	三人浮きのとき
	  トップが +7 point ２位 +5 point ３位 +3 point ４位 -15 point

[記述]
	param: uma = under
[意味]
	沈みウマ

	一人浮きのとき
	  トップが +30 point ２位 -10 point ３位 -10 point ４位 -10 point
	二人浮きのとき
	  トップが +20 point ２位   0 point ３位 -10 point ４位 -10 point
	三人浮きのとき
	  トップが +10 point ２位   0 point ３位   0 point ４位 -10 point

