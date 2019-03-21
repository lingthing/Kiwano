// Copyright (C) 2019 Nomango

#include "easy2d.h"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

class MainScene
	: public easy2d::Scene
{
public:
	MainScene()
	{
		// �������ֽڵ�
		easy2d::TextPtr text = new easy2d::Text(L"Hello Easy2D!");
		// ���ýڵ��СΪ���ֲ��ִ�С
		text->SetSize(text->GetLayoutSize());
		// �޸Ľڵ�λ��, ʹ�ڵ�����Ļ�Ͼ���
		text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		// �޸Ľڵ�ê��, ʹ�������Ķ�����Ļ����
		text->SetAnchor(0.5, 0.5);
		// ���ӵ�������
		this->AddChild(text);
	}
};

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	try
	{
		// ���� Easy2D ����ʵ��
		easy2d::Application app;

		// ������ʼ��ѡ��
		easy2d::Options options;
		// ���ô��ڿ���
		options.width = WINDOW_WIDTH;
		options.height = WINDOW_HEIGHT;
		// ��ʼ�� app
		app.Init(options);

		// ��������������
		easy2d::ScenePtr scene = new MainScene;
		app.EnterScene(scene);

		// ����
		app.Run();
	}
	catch (std::exception& e)
	{
		::MessageBoxA(nullptr, e.what(), "An exception has occurred!", MB_ICONERROR | MB_OK);
	}
	return 0;
}