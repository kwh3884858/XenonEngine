#pragma once

#include "BaseWindow.h"


class MainWindow : public BaseWindow<MainWindow>
{
public:
	MainWindow();
	~MainWindow();

	void Initialize();
	void Shutdown();
	void Run();

	virtual LRESULT HandMessage(UINT uMSG, WPARAM wParam, LPARAM lParam) override;


	protected:

	virtual PCWSTR ClassName() const override { return L"Simple Window Class"; }

};