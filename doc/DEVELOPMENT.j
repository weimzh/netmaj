netmaj 開発資料

このドキュメントは、netmaj 関係の プログラム を 開発するための資料ですが、
まだ十分なものとなっていないため、ヒント程度と考えてください。

I) 概要

netmaj は次のようなクライアント／サーバ構造になっており、
コンピュータプレイヤーを含む各プレイヤー プログラムは、完全に対等になって
います。

サーバは、各プレイヤーが実際のゲームで見れる情報しか送らないように
なっており、コンピュータプレイヤーも不正ができないようになっています。


                        (netmaj プロトコル)
 	                   <--------->   クライアントプログラム1
      サーバプログラム     <--------->   クライアントプログラム2
                           <--------->   クライアントプログラム3
                           <--------->   クライアントプログラム4


II) プログラム構造

プログラム構造は、次のようになっています。

    共通関数
        global.c 	netmaj プロトコル 解析部
	mt.c		山の管理
	rv.c		河の管理
	hand.c		手の管理
	ahand.c		手のメンツ分解
	result.c	役判定
	log.c		ログ機能
	cvt.c		役データ形式変換
	pf.c		パーソナルファイル(パラメータ/戦績)
	pai_str.c	文字定義

    クライアント共通関数
	analize.c	捨て牌選択アルゴリズム

    サーバ
	server.c	メイン
	game.c 		ゲーム進行

    コンピュータプレーヤー
	auto.c 		メイン

III) netmaj プロトコル

サーバ/クライアント間のメッセージには、次のものがあります。
(実際のメッセージ形式については、comm.h を参照)

    サーバ->クライアント

	接続関係
	MSG_CONNECT_DONE	接続完リプライ  
	MSG_CONNECT_PLAYER	接続状況	

	ゲーム進行
	MSG_GAME    		半荘スタート
	MSG_GAMEEND		半荘終了
	MSG_PLAY		局スタート
	MSG_PLAYEND		局終了

	データ送付
	MSG_DORA		ドラ牌送付
	MSG_URADORA		うらドラ牌送付
	MSG_RIVER		捨て牌送付(チー/ポン/カン/ロン 返答を待つ)
	MSG_RIVERIN		捨て牌送付(確定)
	MSG_HANDI		配牌送付
	MSG_HANDC		手牌送付
	MSG_HANDO		さらした手の送付
	MSG_RESULT		局の結果送付
	MSG_POINT		半荘終了時の点数送付
	MSG_ASK			つも牌送付

	TALK 機能
	MSG_COMMENT2		TALK メッセージ送付


    クライアント->サーバ
	
	MSG_CONNECT		接続要求
	MSG_KILL		サーバ終了要求
	MSG_ASK			捨て牌送付
	MSG_REPL		チー/ポン/カン/ロン 返答
	MSG_COMMENT		TALK メッセージ送付

IV) コンピュータプレイヤープログラムインターフェイス

次の関数を作成することにより、全く別のコンピュータプレイヤー
を組み込むことができます。

	analize_init(gp) global_t *gp;
		初期化
	analize(gp,p) global_t *gp; int p;
		捨て牌選択
	analize_res(gp,howp) global_t *gp; int *howp;
		チー/ポン/カン/ロン選択

	(global_t には、捨て牌等 卓上の情報すべてが含まれています)

V) ユーザインターフェイスプログラムインターフェイス

cui.c 中に含まれる プログラムインターフェイスにしたがったプログラムを
作成すれば、キャラクタインターフェイス以外の ユーザインターフェイスを
持ったプログラムを作成できます。

	ゲーム進行上必須のもの
	ui_init(gp)			初期化
	ui_term()			終了処理
	ui_draw(gp,flag)		描画
	ui_menu(gp,key)			メニュー呼出し
	ui_res(gp,howp)			チー/ポン/カン/ロン選択
	ui_choice(gp)			捨て牌/ツモ選択
	ui_key()			キー入力
	ui_event_wait(gp,flag)		select 機能

	メッセージ出力(空関数でもよい)
	ui_message_connecting()
	ui_message_connect()
	ui_message_disconnect()
	ui_message_reach(player)
	ui_message_tumo(player)
	ui_message_ron(player)
	ui_message_player(gp)
	ui_message_gend(gp)
	ui_message_play(gp)
	ui_result(gp)
