#pragma once

class CSelectScene
{
public:
	CSelectScene();
	~CSelectScene();
	void Update();
	void Start();
	void Render();


private:
	Sprite*		m_Select_imag[5];


};

