#include "enemy.h"
#include "Field.h"
#include "debugScreen.h"
#include <assert.h>
#include <cmath>
#include <math.h>
#include "player.h"

namespace
{
	const float JUMP_POWER = 3.0f; // ジャンプ力
	const float JUMP_DOWNPOWER = 0.1f; // ジャンプ力を下げる力
}

Enemy::Enemy(SceneBase * scene) : GameObject(scene)
{
	// モデルの読み込み
	hModel = MV1LoadModel("data\\model\\Enemy\\うさいだーまっ_.pmx");
	assert(hModel > 0);

	// アニメーションの生成
	animation = new Animation(hModel);
	animation->Start(ANIM_ID::NEUTRAL, true);

	// アニメーションで移動をしているフレームの番号を検索する
	MoveAnimFrameIndex = MV1SearchFrame(hModel, "全ての親");

	// アニメーションで移動をしているフレームを無効にする
	MV1SetFrameUserLocalMatrix(hModel, MoveAnimFrameIndex, MGetIdent());

	// ３Ｄモデルの座標を初期化
	Position = VGet(0.0f, 0.0f, 0.0f);
	Rotation = VGet(0.0f, 0.0f, 0.0f);

	Angle = 0.0f;
	Angle2 = 0.0f;

	jumpPower = JUMP_POWER;

	isJump = false;

	TempPosition = Position;

	//カプセルコリジョンの初期化
	p1 = Position;
	p2 = VAdd(Position, VGet(0, 22.0f, 0));

	rad = 3.0f;
}

Enemy::~Enemy()
{
	// モデルの削除
	if (hModel > 0)
		MV1DeleteModel(hModel);
}

void Enemy::Start()
{
}

void Enemy::Update()
{
	// 地面との当たり判定
	HitField();

	//// ステータスの更新
	switch (status)
	{
	case STATE_NEUTRAL:
	case STATE_MOVE:
		Move();
		break;
	case STATE_WASATTACK:
		WasAttack();
		break;
	case STATE_DOWN:
		Down();
		break;
	case STATE_DOWN_LOOP:
		DownLoop();
		break;
	case STATE_STANDUP:
		StandUp();
		break;
	}

	// アニメーションのアップデート
	animation->Update();

	p1 = Position;
	p2 = VAdd(Position, VGet(0, 22.0f, 0));
}

void Enemy::Draw()
{
	MATRIX m;

	//// 移動の行列を作る
	MATRIX mTrans = MGetTranslate(Position);

	//// Y軸回転の行列を作る
	MATRIX mRotY = MGetRotY(Rotation.y);

	//// 回転行列と平行移動行列
	m = MMult(mRotY, mTrans);

	//// キャラクターモデルの行列の設定
	MV1SetMatrix(hModel, m);

	// キャラクターモデル描画
	MV1DrawModel(hModel);

	DrawCapsule3D(p1, p2, rad, 40, 8, GetColor(255, 255, 0), FALSE);
}

/// <summary>
/// 当たっていればtrue
/// </summary>
bool Enemy::HitCollision(VECTOR _cpos, float _crad,bool _downAttack)
{
	float len;//最短距離
	float arad = _crad + rad;//半径（球体）+ 半径（カプセル）
	Player *pl = GetScene()->FindGameObject<Player>();
	VECTOR PlayerPosition = VNorm(VSub(pl->GetPosition(),Position));
	//カプセルの
	//上
	if (_cpos.y >= p2.y)
	{
		len = VSize(VSub(p2, _cpos));
		//距離が半径（球体）+ 半径（カプセル）以下 
		if (len <= arad)
		{
			if (_downAttack == true)
			{
				Rotation.y = atan2(PlayerPosition.x* -1.0f,PlayerPosition.z* -1.0f);
				animation->Start(ANIM_ID::DOWN, false);
				status = STATE_DOWN;
				return true;
			}
			animation->Start(ANIM_ID::KNOCKBACK, false);
			animation->ResetTime();
			status = STATE_WASATTACK;
			return true;
		}
		return false;
	}
	//下
	if (_cpos.y <= p1.y)
	{
		len = VSize(VSub(p1, _cpos));
		//距離が半径（球体）+ 半径（カプセル）以下 
		if (len <= arad)
		{
			if (_downAttack == true)
			{
				Rotation.y = atan2(PlayerPosition.x* -1.0f,PlayerPosition.z* -1.0f);
				animation->Start(ANIM_ID::DOWN, false);
				status = STATE_DOWN;
				return true;
			}
			animation->Start(ANIM_ID::KNOCKBACK, false);
			animation->ResetTime();
			status = STATE_WASATTACK;
			return true;
		}
		return false;
	}

	//距離を測る
	len = VSize(VSub(VGet(p1.x,0.0f,p1.z), VGet(_cpos.x,0.0f,_cpos.z)));

	//距離より半径（球体）+ 半径（カプセル）の方が大きければ 
	if (arad > len)
	{
		if (_downAttack == true)
		{
			Rotation.y = atan2(PlayerPosition.x* -1.0f,PlayerPosition.z* -1.0f);
			animation->Start(ANIM_ID::DOWN, false);
			status = STATE_DOWN;
			return true;
		}
		animation->Start(ANIM_ID::KNOCKBACK, false);
		animation->ResetTime();
		status = STATE_WASATTACK;
		return true;
	}
	return false;
}

VECTOR Enemy::CollisionPlayer(VECTOR center, float r, VECTOR vel)
{
	VECTOR v2 = VSub(Position, center); // プレイヤーから敵へのベクトル
	v2.y = 0.0f; // Yの差を0にしておく(円柱で当てる)

	if (VSize(v2) > r + rad) { // 当たってない
		return center;
	}
	v2 = VNorm(v2); // 長さを１にする
	float push = VDot(vel, v2); // 押した量
	if (push <= 0.0f) // 反対向きなので処理しない
		return center;
	VECTOR enePush = VScale(v2, push); // 敵を押す量
	VECTOR playerPush = VSub(vel, enePush);
	Position = VAdd(Position, enePush);
	center = VSub(center, vel); // 移動前の位置に戻し
	return VAdd(center, playerPush); // 移動後の位置を再計算
}

void Enemy::Move()
{
	// 待機アニメーションを再生
	animation->Start(ANIM_ID::NEUTRAL, true);
}

void Enemy::Walk()
{
	// 何も操作していなければ待機状態に戻す
	if (CheckHitKey(KEY_INPUT_W) == 0 &&
		CheckHitKey(KEY_INPUT_A) == 0 &&
		CheckHitKey(KEY_INPUT_S) == 0 &&
		CheckHitKey(KEY_INPUT_D) == 0)
	{
		beforestatus = STATE_WALK;
		status = STATE_NEUTRAL;
		return;
	}

	// キーを押すことで走りの処理を実行
	if (CheckHitKey(KEY_INPUT_LSHIFT) != 0)
	{
		beforestatus = STATE_WALK;
		status = STATE_RUN;
		return;
	}

	// 歩きアニメーションを再生
	animation->Start(ANIM_ID::WALK, true);

	// カメラの向きに合わせてキャラクターの向きを変える
	SetCharacterRotationY();

	MATRIX rotY = MGetRotY(Rotation.y); //回転行列を作る


	// 移動の処理
	VECTOR vel = VTransform(VGet(0, 0, -1.0f), rotY);	//回転行列を使ったベクトルの変換
	Position = VAdd(Position, vel);	//座標に移動量を足す

	// ジャンプを実行
	if (CheckHitKey(KEY_INPUT_SPACE) != 0)
	{
		// ジャンプアニメーションを再生
		animation->Start(ANIM_ID::JUMP, false);
		jumpPower = JUMP_POWER;
		Position.y += jumpPower;
		isJump = true;
		beforestatus = STATE_WALK;
		status = STATE_JUMP;
		return;
	}

	// 連続攻撃を実行
	if (CheckHitKey(KEY_INPUT_RETURN) != 0)
	{
		animation->Start(ANIM_ID::ATTACK_1, false);
		ComboNum = 1;
		status = STATE_COMBOATTACK;
		return;
	}

	//回避を実行
	if (CheckHitKey(KEY_INPUT_C) != 0)
	{
		animation->Start(ANIM_ID::DODGE, false);
		status = STATE_DODGE;
		return;
	}


	// ガードを実行
	if (CheckHitKey(KEY_INPUT_G) != 0)
	{
		animation->Start(ANIM_ID::GUARD, false);
		status = STATE_GUARD;
		return;
	}
}

void Enemy::Run()
{
	// キーを押していなければ歩きの処理を実行
	if (CheckHitKey(KEY_INPUT_LSHIFT) == 0)
	{
		beforestatus = STATE_RUN;
		status = STATE_WALK;
	}

	// 走りアニメーションを再生
	animation->Start(ANIM_ID::RUN, true);

	// カメラの向きに合わせてキャラクターの向きを変える
	SetCharacterRotationY();

	MATRIX rotY = MGetRotY(Rotation.y); //回転行列を作る

	// 移動の処理
	VECTOR vel = VTransform(VGet(0, 0, -5.0f), rotY);	// 回転行列を使ったベクトルの変換
	Position = VAdd(Position, vel);	// 座標に移動量を足す

	// ジャンプを実行
	if (CheckHitKey(KEY_INPUT_SPACE) != 0)
	{
		//ジャンプアニメーションを再生
		animation->Start(ANIM_ID::JUMP, false);
		jumpPower = JUMP_POWER;
		Position.y += jumpPower;
		isJump = true;
		beforestatus = STATE_RUN;
		status = STATE_JUMP;
	}

	// ダッシュ攻撃を実行
	if (CheckHitKey(KEY_INPUT_RETURN) != 0)
	{
		animation->Start(ANIM_ID::TACKLE, false);
		beforestatus = STATE_DASHATTACK;
		status = STATE_DASHATTACK;
		return;
	}

	//回避を実行
	if (CheckHitKey(KEY_INPUT_C) != 0)
	{
		animation->Start(ANIM_ID::DODGE, false);
		status = STATE_DODGE;
		return;
	}
}

void Enemy::Jump()
{
	// 地面についたら処理をやめる
	if (Position.y <= hit.y)
	{
		isJump = false;
		status = beforestatus;
	}

	// 処理
	// ジャンプ力を減らす
	jumpPower -= JUMP_DOWNPOWER;
	//座標にジャンプ力を与える
	Position.y += jumpPower;

	MATRIX rotY = MGetRotY(Rotation.y);

	// 座標を移動
	if (beforestatus == STATE_WALK)
	{
		VECTOR vel = VTransform(VGet(0, 0, -1.0f), rotY);	// 回転行列を使ったベクトルの変換
		Position = VAdd(Position, vel);	// 座標に移動量を足す
	}
	if (beforestatus == STATE_RUN)
	{
		VECTOR vel = VTransform(VGet(0, 0, -5.0f), rotY);	// 回転行列を使ったベクトルの変換
		Position = VAdd(Position, vel);	// 座標に移動量を足す
	}

	//　ジャンプ連続攻撃を実行
	// 連続攻撃を実行
	if (CheckHitKey(KEY_INPUT_RETURN) != 0)
	{
		animation->Start(ANIM_ID::ATTACK_1, false);
		ComboNum = 1;
		status = STATE_JUMPCOMBOATTACK;
		return;
	}

	// ガードを実行
	if (CheckHitKey(KEY_INPUT_G) != 0)
	{
		animation->Start(ANIM_ID::GUARD, false);
		status = STATE_JUMPGUARD;
		return;
	}
}

void Enemy::ComboAttack()
{
	// 連続何回目かで攻撃が変わる
	switch (ComboNum)
	{
	case 1:
		if (CheckHitKey(KEY_INPUT_RETURN) != 0)
		{
			if (animation->GetFrameTime() > 15.0f)
			{
				ComboNum = 2;
				animation->Start(ANIM_ID::ATTACK_2, false);
			}
		}
		if (animation->IsFinish() == true)
		{
			status = STATE_NEUTRAL;
		}
		break;
	case 2:
		if (CheckHitKey(KEY_INPUT_RETURN) != 0)
		{
			if (animation->GetFrameTime() > 5.0f)
			{
				ComboNum = 3;
				animation->Start(ANIM_ID::ATTACK_3, false);
			}
		}
		if (animation->IsFinish() == true)
		{
			status = STATE_NEUTRAL;
		}
		break;
	case 3:
		if (animation->IsFinish() == true)
		{
			status = STATE_NEUTRAL;
		}
		break;
	}
}

void Enemy::DashAttack()
{
	if (animation->IsFinish() == true)
	{
		status = STATE_NEUTRAL;
	}
	//移動量の分向いている方向に進める

	if (animation->GetFrameTime() < 5.0f || animation->GetFrameTime() > 20.0f)
	{
		return;
	}

	MATRIX rotY = MGetRotY(Rotation.y);

	VECTOR vel = VTransform(VGet(0, 0, -3.0f), rotY);	// 回転行列を使ったベクトルの変換
	Position = VAdd(Position, vel);	// 座標に移動量を足す

}

void Enemy::JumpComboAttack()
{
	// 地面についたら処理をやめる
	if (Position.y <= hit.y)
	{
		isJump = false;
		status = STATE_NEUTRAL;
	}

	// 処理
	// ジャンプ力を減らす
	jumpPower -= JUMP_DOWNPOWER;
	Position.y += jumpPower;

	// 連続何回目かで攻撃が変わる
	switch (ComboNum)
	{
	case 1:
		if (CheckHitKey(KEY_INPUT_RETURN) != 0)
		{
			if (animation->GetFrameTime() > 15.0f)
			{
				ComboNum = 2;
				animation->Start(ANIM_ID::ATTACK_2, false);
			}
		}
		if (animation->IsFinish() == true)
		{
			status = STATE_JUMP;
		}
		break;
	case 2:
		if (CheckHitKey(KEY_INPUT_RETURN) != 0)
		{
			if (animation->GetFrameTime() > 5.0f)
			{
				ComboNum = 3;
				animation->Start(ANIM_ID::ATTACK_3, false);
			}
		}
		if (animation->IsFinish() == true)
		{
			status = STATE_JUMP;
		}
		break;
	case 3:
		if (animation->IsFinish() == true)
		{
			status = STATE_JUMP;
		}
		break;
	}
}

void Enemy::Dodge()
{
	if (animation->GetFrameTime() > 5.0f)
	{
		status = STATE_NEUTRAL;
	}
	if (animation->GetFrameTime() > 3.0f)
	{
		return;
	}
	MATRIX rotY = MGetRotY(Rotation.y);
	VECTOR vel = VTransform(VGet(0, 0, -10.0f), rotY);	// 回転行列を使ったベクトルの変換
	Position = VAdd(Position, vel);	// 座標に移動量を足す
}

void Enemy::Guard()
{
	if (CheckHitKey(KEY_INPUT_G) != 0)
	{
		return;
	}
	status = STATE_NEUTRAL;
}

void Enemy::JumpGuard()
{
	// 地面についたら処理をやめる
	if (Position.y <= hit.y)
	{
		isJump = false;
		status = STATE_NEUTRAL;
	}

	// 処理
	// ジャンプ力を減らす
	jumpPower -= JUMP_DOWNPOWER;
	Position.y += jumpPower;
	if (CheckHitKey(KEY_INPUT_G) != 0)
	{
		return;
	}
	animation->Start(ANIM_ID::JUMP, true);
	status = STATE_JUMP;

}

void Enemy::WasAttack()
{
	if (animation->IsFinish() == true)
	{
		animation->Start(ANIM_ID::NEUTRAL,true);
		status = STATE_NEUTRAL;
	}
}

void Enemy::Down()
{
	MATRIX rotY = MGetRotY(Rotation.y);

	VECTOR vel = VTransform(VGet(0, 0, 1.0f), rotY);	// 回転行列を使ったベクトルの変換
	Position = VAdd(Position, vel);	// 座標に移動量を足す
	if (animation->IsFinish() == true)
	{
		animation->Start(ANIM_ID::STANDUP, false);
		status = STATE_STANDUP;
	}
}

void Enemy::DownLoop()
{
}

void Enemy::StandUp()
{
	if (animation->IsFinish() == true)
	{
		animation->Start(ANIM_ID::NEUTRAL, false);
		status = STATE_NEUTRAL;
	}
}

void Enemy::HitField()
{
	// 地面との当たり判定の為のRayを作成
	VECTOR upper = VAdd(Position, VGet(0, 1000, 0));
	VECTOR lower = VAdd(Position, VGet(0, -1000, 0));

	// ステージを読み込み
	Field* field = GetScene()->FindGameObject<Field>();

	//　地面との当たり判定
	if (field->CollisionLine(&hit, upper, lower))
	{
		//　ジャンプしていなければ座標を地面と座標を合わせる
		if (isJump == false)
		{
			Position = hit;
		}
	}
	else
	{
		// 足元に地面が無ければ降下
		jumpPower -= JUMP_DOWNPOWER;
		Position.y += jumpPower;
	}
}

void Enemy::SetCamera()
{
}

void Enemy::SetCharacterRotationY()
{
}
