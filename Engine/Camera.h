#pragma once
#include "Component.h"
#include "Frustum.h"

enum class PROJECTION_TYPE
{
	PERSPECTIVE,		// 원근투영
	ORTHOGRAPHIC,		// 직교투영
};

/*-----------
	Camera
------------*/
class Camera : public Component
{
public:
	/* ----- Constroct Function ----- */
	Camera();
	virtual ~Camera();

	/* ----- External Function ----- */
	virtual void		FinalUpdate() override;
	//void				Render();

	void SortGameObject();
	void Render_Deferred();
	void Render_Forward();

	/* ----- Helper Function ----- */
	float				GetNear() { return _near; }
	float				GetFar() { return _far; }
	float				GetFov() { return _fov; }
	PROJECTION_TYPE		GetProjectionType() { return _type; }

	void				SetProjectionType(PROJECTION_TYPE type) { _type = type; }
	void				SetNear(float value) { _near = value; }
	void				SetFar(float value) { _far = value; }
	void				SetFov(float value) { if (value < 0.1f) { value = 0.1f; } else if (value >= XM_PI) { value = XM_PI; }_fov = value; }	// 귀찮아서 한줄로 코딩함
	
	void SetCullingMaskLayerOnOff(uint8 layer, bool flag)
	{
		if (flag) {
			_cullingMask |= (1 << layer);
		}
		else {
			_cullingMask &= ~(1 << layer);
		}
	}
	void SetCullingMaskLayerOnOff(LAYER_TYPE layer, bool flag) { SetCullingMaskLayerOnOff(static_cast<uint8>(layer),flag); }
	
	void				SetCullingMaskAll() { SetCullingMask(UINT32_MAX); }
	void				SetCullingMask(uint32 mask) { _cullingMask = mask; }
	bool				IsCulled(uint8 layer) { return (_cullingMask & (1 << layer)) != 0; }
private:
	PROJECTION_TYPE		_type = PROJECTION_TYPE::PERSPECTIVE;

	float				_near = 1.0f;
	float				_far = 2000.0f;
	float				_fov = XM_PI / 4.0f;
	float				_scale = 1.0f;

	Matrix				_matView = {};
	Matrix				_matProjection = {};

	Frustum				_frustum;
	uint32				_cullingMask = 0;

private:
	vector<shared_ptr<GameObject>> _vecDeferred;
	vector<shared_ptr<GameObject>> _vecForward;
	vector<shared_ptr<GameObject>> _vecParticle;

public:
	static Matrix		S_MatView;			
	static Matrix		S_MatProjection;		// 외부에서 쓰기 편하게 만든녀석
};

