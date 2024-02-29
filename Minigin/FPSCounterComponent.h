#pragma once
#include <string>
#include <memory>
#include "TextObjectComponent.h"
#include <chrono>

namespace dae
{
	class Font;
	class Texture2D;
	class FPSCounterComponent final : public TextObjectComponent
	{
	public:
		FPSCounterComponent(const std::string& text, std::shared_ptr<Font> font);
		virtual ~FPSCounterComponent() = default;
		FPSCounterComponent(const FPSCounterComponent& other) = delete;
		FPSCounterComponent(FPSCounterComponent&& other) = delete;
		FPSCounterComponent& operator=(const FPSCounterComponent& other) = delete;
		FPSCounterComponent& operator=(FPSCounterComponent&& other) = delete;

		virtual void Update(float deltaTime) override;
		void Init() override;
		virtual void FixedUpdate() override;
		void Render() const override;

		void SetPosition(float x, float y);


	private:
		std::chrono::steady_clock::time_point m_EndTime;
		std::chrono::steady_clock::time_point m_StartTime;
		float m_ElapsedTime{ 0 };
		int m_FPS{ 0 };
	};
}