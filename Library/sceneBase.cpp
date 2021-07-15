#include "sceneBase.h"
#include "gameObject.h"

SceneBase::SceneBase()
{
	m_objects.clear();
	m_drawObjects.clear();
	m_needSort = false;
}

SceneBase::~SceneBase()
{
	DeleteAllGameObject();
	m_needSort = false;
}

void SceneBase::Update()
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ) {
		NODE& node = (*it);
		if (!node.initialized) {
			node.object->Start();
			node.initialized = true;
		}
		if (node.object->IsDestroy()) {
			DeleteFromDrawObjects(node.object);
			delete node.object;
			it = m_objects.erase(it);
		} else {
			node.object->Update();
			it++;
		}
	}
}

void SceneBase::Draw()
{
	if (m_needSort) {
		m_drawObjects.clear();
		m_drawObjects = m_objects; // Listを全コピー
		m_drawObjects.sort(Comp); // 昇順にソート
		m_needSort = false;
	}
	
	for (auto& node : m_drawObjects)
		node.object->Draw();
}

void SceneBase::SetDrawOrder(GameObject * _obj, int _order)
{
	for (auto& node : m_objects) {
		if (node.object == _obj) {
			node.order = _order;
			m_needSort = true;
		}
	}
}

void SceneBase::DeleteGameObject(GameObject * obj)
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ) {
		NODE& node = *it;
		if (node.object == obj) {
			DeleteFromDrawObjects(obj);
			delete node.object;
			m_objects.erase(it);
			return;
		}
		it++;
	}
}

void SceneBase::DeleteAllGameObject()
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ) {
		delete (*it).object;
		it = m_objects.erase(it);
	}
	for (auto it = m_drawObjects.begin(); it != m_drawObjects.end(); ) {
		it = m_drawObjects.erase(it);
	}
}

std::list<GameObject*> SceneBase::GetAllObjects() const
{
	std::list<GameObject*> list;

	list.clear();
	for (auto& node : m_objects)
		list.push_back(node.object);
	return list;
}

void SceneBase::DeleteFromDrawObjects(GameObject * obj)
{
	for (auto it = m_drawObjects.begin(); it != m_drawObjects.end(); ) {
		NODE& node = *it;
		if (node.object == obj) {
			it = m_drawObjects.erase(it);
			return;
		}
		it++;
	}
}

bool SceneBase::Comp(const NODE & o1, const NODE & o2)
{
	return o1.order < o2.order;
}
