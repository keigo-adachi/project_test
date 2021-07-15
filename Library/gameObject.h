#pragma once
#include <DxLib.h>
#include <string>
#include "sceneBase.h"

class GameObject {
public:
	GameObject() : m_pScene(nullptr), m_pParent(nullptr), m_destroy(false), m_tag("") {}
	GameObject(SceneBase* scene) : m_pScene(scene), m_pParent(nullptr), m_destroy(false), m_tag("") {}
	GameObject(GameObject* object) : m_pScene(nullptr), m_pParent(object), m_destroy(false), m_tag("") {}
	GameObject(SceneBase* scene, GameObject* object) : m_pScene(scene), m_pParent(object), m_destroy(false), m_tag("") {}
	virtual ~GameObject() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void Draw() {}

	/// <summary>
	/// 自分のインスタンスを削除する時に呼び出す
	/// 次回のUpdateの前に削除される
	/// </summary>
	void DestroyMe() { m_destroy = true; }

	/// <summary>
	/// インスタンスが削除要求されているかを調べる
	/// 主に、SceneManagerでチェックしている
	/// </summary>
	/// <returns></returns>
	bool IsDestroy() const { return m_destroy; }

	/// <summary>
	/// タグをつける
	/// タグは１つだけ付けることができます
	/// </summary>
	/// <param name="_tag">タグ</param>
	void SetTag(std::string _tag) { m_tag = _tag; }

	/// <summary>
	/// 指定されたタグと同じかを返す
	/// </summary>
	/// <param name="_tag">タグ</param>
	/// <returns>同じであればtrue</returns>
	bool IsTag(std::string _tag) const { return m_tag == _tag; }

	/// <summary>
	/// タグを取得する
	/// </summary>
	/// <returns>設定されているタグ</returns>
	const std::string& GetTag() const { return m_tag; }

	/// <summary>
	/// 所属するシーンを返す
	/// </summary>
	/// <returns>所属するシーンのポインター</returns>
	SceneBase* GetScene() const { return m_pScene; }

	/// <summary>
	/// シーンのポインターを保存する
	/// </summary>
	/// <returns>所属するシーンのポインター</returns>
	void SetScene(SceneBase* _scene) { m_pScene = _scene; }

	/// <summary>
	/// 親のオブジェクトを返す
	/// </summary>
	/// <returns>親オブジェクトのポインター</returns>
	GameObject* GetParent() const { return m_pParent; }

	/// <summary>
	/// 親のオブジェクトを返す
	/// </summary>
	/// <returns>親オブジェクトのポインター</returns>
	void SetParent(GameObject* _parent) { m_pParent = _parent; }

private:
	SceneBase* m_pScene;   // 所属するシーン
	GameObject* m_pParent; // 親のオブジェクト
	bool m_destroy;        // 自分を削除するか否か
	std::string m_tag;     // タグ
};
