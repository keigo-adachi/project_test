#pragma once
#include "../Library//gameObject.h"
#include "animation.h"
#include "camera.h"
#include "enemy.h"

class Player :public GameObject{
public:
	//アニメーション
	enum ANIM_ID {
		NEUTRAL = 0,	//待機
		WALK_START,		//歩き出し
		WALK,			//歩き
		WALK_END,		//歩き止まり
		RUN_START,		//走り出し
		RUN,			//走り
		RUNEND,			//走り止まり
		TACKLE,			//突進
		DOWN,			//吹き飛ばされ
		STANDUP,		//立ち上がり
		ATTACK_1,		//通常攻撃(1回目)
		ATTACK_2,		//通常攻撃(2回目)
		ATTACK_3,		//通常攻撃(3回目)
		GUARD,			//ガード
		GUARD_SUCCES,	//ガード成功
		KNOCKBACK,		//のけぞり
		JUMP_START,		//ジャンプ始め
		JUMP,			//ジャンプ
		JUMP_END,		//ジャンプ終わり
		DODGE,			//回避
		DOWN_LOOP,		//倒れ
		MAX
	};

	Player(SceneBase*scene);
	~Player();
	void Start()override;
	void Update()override;
	void Draw()override;

	VECTOR GetPosition() { return Position; }; //プレイヤーの座標を取得
	
private:
	//ステータス
	enum STATE
	{
		STATE_NEUTRAL = 0,		// 待機
		STATE_MOVE,				// 行動を決める
		STATE_WALK,				// 歩き
		STATE_RUN,				// 走り
		STATE_JUMP,				// ジャンプ
		STATE_COMBOATTACK,		// 連続攻撃
		STATE_DASHATTACK,		// ダッシュ攻撃
		STATE_JUMPCOMBOATTACK,  // ジャンプ連続攻撃
		STATE_DODGE,			// 回避
		STATE_GUARD,			// ガード
		STATE_JUMPGUARD,		// ガード（ジャンプ中）
		STATE_WASATTACK,		// 攻撃を受けた
		STATE_DOWN,				// ダウンした
	};
	STATE status; // 現在のステータス
	STATE beforestatus; // ひとつ前のステータス

	// ステータスによる処理
	void Move();
	void Walk();
	void Run();
	void Jump();
	void ComboAttack();
	void DashAttack();
	void JumpComboAttack();
	void Dodge();
	void Guard();
	void JumpGuard();
	void WasAttack();
	void Down();

	void HitField();
	void SetCamera();
	void SetCharacterRotationY();

	Animation* animation;
	Camera*Cm;
	Enemy* enemy;

	int hModel; // モデルのハンドラー

	int MoveAnimFrameIndex;
	VECTOR Position; // ワールド座標
	VECTOR TempPosition; // 一時的なワールド座標
	VECTOR NowPosition; // ローカル座標現在の位置
	VECTOR PrevPosition; // ローカル座標原点座標
	VECTOR Rotation; // 回転値
	VECTOR Vel;	//移動量

	float YawAngle; // 回転値(横)
	float PitchAngle; // 回転値(縦)

	VECTOR CPos; // カメラ座標

	bool isJump; // ジャンプしていればtrue

	float jumpPower; // ジャンプ力

	VECTOR hit; // Rayと地面との接地

	int ComboNum; //攻撃回数

	//攻撃時の当たり判定
	struct ATTACKCOLLISION
	{
		VECTOR Position;	//座標
		float Radius;		//半径
		int FrameIndex;		//アタッチするフレームの番号
		bool isEnable;		//有効かどうか
		bool DownAttack;	//ダウンする攻撃かどうか
	};
	ATTACKCOLLISION  Collision[4]; //拳（左右）0,1、足(右）2,胴体 3
};