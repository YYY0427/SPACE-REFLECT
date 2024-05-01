#include "Image3D.h"
#include "Util/DrawFunctions.h"
#include <cassert>

// コンストラクタ
Image3D::Image3D(const std::string& filePath) :
	m_pos({}),
	m_rot({}),
	m_imgWidth(0),
	m_imgHeight(0),
	m_vertex({}),
	m_alpha(255)
{
	// 画像の読み込み
	m_imgHandle = my::MyLoadGraph(filePath.c_str());
}

// コンストラクタ
Image3D::Image3D(const int imgHandle) :
	m_imgHandle(imgHandle),
	m_pos({}),
	m_rot({}),
	m_imgWidth(0),
	m_imgHeight(0),
	m_vertex({}),
	m_alpha(255)
{
	// 画像ハンドルが無効なら止める
	assert(m_imgHandle != -1);
}

// デストラクタ
Image3D::~Image3D()
{
	// 画像の削除
	DeleteGraph(m_imgHandle);
}

// 更新
void Image3D::Update()
{
	// 各頂点の情報を設定
	m_vertex[0].dif = GetColorU8(255, 255, 255, 255);
	m_vertex[0].spc = GetColorU8(0, 0, 0, 0);
	m_vertex[0].u = 0.0f;
	m_vertex[0].v = 0.0f;
	m_vertex[0].su = 0.0f;
	m_vertex[0].sv = 0.0f;

	m_vertex[1].dif = GetColorU8(255, 255, 255, 255);
	m_vertex[1].spc = GetColorU8(0, 0, 0, 0);
	m_vertex[1].u = 1.0f;
	m_vertex[1].v = 0.0f;
	m_vertex[1].su = 0.0f;
	m_vertex[1].sv = 0.0f;

	m_vertex[2].dif = GetColorU8(255, 255, 255, 255);
	m_vertex[2].spc = GetColorU8(0, 0, 0, 0);
	m_vertex[2].u = 0.0f;
	m_vertex[2].v = 1.0f;
	m_vertex[2].su = 0.0f;
	m_vertex[2].sv = 0.0f;

	m_vertex[3].dif = GetColorU8(255, 255, 255, 255);
	m_vertex[3].spc = GetColorU8(0, 0, 0, 0);
	m_vertex[3].u = 1.0f;
	m_vertex[3].v = 1.0f;
	m_vertex[3].su = 0.0f;
	m_vertex[3].sv = 0.0f;

	m_vertex[4] = m_vertex[2];
	m_vertex[5] = m_vertex[1];

	// 回転( x, y, z軸回転の順 )＋座標移動行列の作成
	MATRIX transformMatrix;
	transformMatrix = MGetRotX(m_rot.x);
	transformMatrix = MMult(transformMatrix, MGetRotY(m_rot.y));
	transformMatrix = MMult(transformMatrix, MGetRotZ(m_rot.z));
	transformMatrix = MMult(transformMatrix, MGetTranslate(VGet(m_pos.x, m_pos.y, m_pos.z)));

	// 行列を使ってワールド座標を算出
	m_vertex[0].pos = VTransform(VGet(-m_imgWidth, m_imgHeight, 0.0f), transformMatrix);	// 左上
	m_vertex[1].pos = VTransform(VGet(m_imgWidth, m_imgHeight, 0.0f), transformMatrix);		// 右上
	m_vertex[2].pos = VTransform(VGet(-m_imgWidth, -m_imgHeight, 0.0f), transformMatrix);	// 左下
	m_vertex[3].pos = VTransform(VGet(m_imgWidth, -m_imgHeight, 0.0f), transformMatrix);	// 右下

	m_vertex[4].pos = m_vertex[2].pos;
	m_vertex[5].pos = m_vertex[1].pos;

	// 行列を使って法線を算出
	m_vertex[0].norm = VTransformSR(VGet(0.0f, 0.0f, -1.0f), transformMatrix);
	m_vertex[1].norm = m_vertex[0].norm;
	m_vertex[2].norm = m_vertex[0].norm;
	m_vertex[3].norm = m_vertex[0].norm;
	m_vertex[4].norm = m_vertex[0].norm;
	m_vertex[5].norm = m_vertex[0].norm;
}

// 描画
void Image3D::Draw()
{
	// ポリゴンの裏側も描画する
	SetUseBackCulling(FALSE);

	// ライティングは行わない
	SetUseLighting(FALSE);

	// 透明度を設定
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);

	// ２ポリゴンの描画
	DrawPolygon3D(m_vertex.data(), 2, m_imgHandle, true);

	// 透明度を戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ライティングは行わない
	SetUseLighting(TRUE);

	// ポリゴンの裏側は描画しない
	SetUseBackCulling(TRUE);
}

// 画像ハンドルの取得
int Image3D::GetImageHandle() const
{
	return m_imgHandle;
}

// 画像の頂点情報の取得
const std::array<VERTEX3D, 6>& Image3D::GetVertex() const
{
	return m_vertex;
}

// 位置の設定
void Image3D::SetPos(const Vector3& pos)
{
	m_pos = pos;
}

// 回転の設定
void Image3D::SetRot(const Vector3& rot)
{
	m_rot = rot;
}

// 画像の横幅の設定
void Image3D::SetImgWidth(const float width)
{
	m_imgWidth = width;
}

// 画像の縦幅の設定
void Image3D::SetImgHeight(const float height)
{
	m_imgHeight = height;
}

// 画像の透明度の設定
void Image3D::SetAlpha(const int alpha)
{
	m_alpha = alpha;
}

// 画像の横幅と立幅の拡大率を元の画像のサイズに戻す
void Image3D::SetImgDafualtScale()
{
	// 画像の大きさどおり画像を描画する
	GetGraphSizeF(m_imgHandle, &m_imgWidth, &m_imgHeight);
}