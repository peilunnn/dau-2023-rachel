#include "stdafx.h"
#include "Components/include/Animation.h"
#include "Components/include/Renderable.h"
#include "Components/include/Transform.h"
#include "Components/include/Score.h"
#include "Components/include/Timer.h"
#include "Systems/include/ScreenHandler.h"
#include "Utilities/include/Helper.h"
#include "Utilities/include/App.h"
#include "../include/RenderingHandler.h"

void RenderingHandler::Render(EntityManager& entityManager)
{
	for (auto entity : entityManager.GetEntitiesWithComponents<Tag, Transform>())
	{
		auto tag = entityManager.GetComponent<Tag>(entity);
		auto transform = entityManager.GetComponent<Transform>(entity);

		// Render sprites
		auto renderable = entityManager.GetComponent<Renderable>(entity);
		if (renderable)
		{
			renderable->GetSprite()->SetPosition(transform->GetPosition().x, transform->GetPosition().y);
			renderable->GetSprite()->SetScale(transform->GetScale().x);

			if (tag->GetEntityType() == EntityType::AMMO_FILLED)
			{
				bool isVisible = renderable->GetSprite()->GetIsVisible();
				if (!isVisible)
					continue;
			}

			renderable->GetSprite()->Draw();
		}

		// Render score
		auto score = entityManager.GetComponent<Score>(entity);
		if (score && tag->GetEntityType() == EntityType::SCORE)
		{
			string scoreText = "Score: " + to_string(score->GetScore());
			float x = transform->GetPosition().x;
			float y = transform->GetPosition().y;
			App::Print(x, y, scoreText.c_str(), 1.0f, 1.0f, 1.0f);
		}

		// Render timer
		auto timer = entityManager.GetComponent<Timer>(entity);
		if (timer && tag->GetEntityType() == EntityType::TIMER)
		{
			string timerText = to_string(static_cast<int>(timer->GetCountdownTime()));
			float x = transform->GetPosition().x;
			float y = transform->GetPosition().y;
			App::Print(x, y, timerText.c_str(), 1.0f, 1.0f, 1.0f);
		}
	}
}

void RenderingHandler::SetBackground(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	glClearColor(red, green, blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderingHandler::DrawBorder(GLfloat red, GLfloat green, GLfloat blue)
{
	constexpr float borderThickness = ScreenHandler::BORDER_THICKNESS;
	constexpr float borderLeftX = ScreenHandler::BORDER_LEFT_X;
	constexpr float borderRightX = ScreenHandler::BORDER_RIGHT_X;
	constexpr float borderTopY = ScreenHandler::BORDER_TOP_Y;
	constexpr float borderBottomY = ScreenHandler::BORDER_BOTTOM_Y;

	glColor3f(red, green, blue);
	glLineWidth(borderThickness);
	glBegin(GL_LINE_LOOP);
	glVertex2f(borderLeftX, borderTopY);
	glVertex2f(borderRightX, borderTopY);
	glVertex2f(borderRightX, borderBottomY);
	glVertex2f(borderLeftX, borderBottomY);
	glEnd();
}

void RenderingHandler::DrawBackgroundInBorder(GLfloat red, GLfloat green, GLfloat blue)
{
	float borderThickness = ScreenHandler::BORDER_THICKNESS;
	float borderLeftX = ScreenHandler::BORDER_LEFT_X;
	float borderRightX = ScreenHandler::BORDER_RIGHT_X;
	float borderTopY = ScreenHandler::BORDER_TOP_Y;
	float borderBottomY = ScreenHandler::BORDER_BOTTOM_Y;

	glColor3f(red, green, blue);
	glBegin(GL_QUADS);
	glVertex2f(borderLeftX, borderTopY);
	glVertex2f(borderRightX, borderTopY);
	glVertex2f(borderRightX, borderBottomY);
	glVertex2f(borderLeftX, borderBottomY);
	glEnd();
}