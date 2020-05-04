#include "precomp.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include "Renderer.h"
#include "Render/Window.h"
#include "Render/Texture.h"
#include "ECS/Entity.h"



namespace Engine
{

	bool Renderer::Init(const WindowData& windowData_)
	{
		LOG_INFO("Initializing Renderer");

		
		m_Window = std::make_unique<Window>();

		if (!m_Window->Init(windowData_))
		{
			LOG_CRITICAL("Unable to create a Window.");
		}

		m_NativeRenderer = SDL_CreateRenderer(
			m_Window->GetNativeWindowHandle(),
			-1,
			windowData_.m_Vsync ? SDL_RENDERER_PRESENTVSYNC : 0 |
			SDL_RENDERER_ACCELERATED);

		if (m_NativeRenderer == nullptr)
		{
			LOG_CRITICAL("Unable to create a renderer. SDL error: {}", SDL_GetError());
			return false;
		}
		
		SetBackgroundColor(100, 150, 236, SDL_ALPHA_OPAQUE);

		
		SDL_GetRendererInfo(m_NativeRenderer, &m_RendererInfo);


		LOG_INFO("RenderSystem initialized successfully");

		// Init font for text
		TTF_Init();
		m_Font = TTF_OpenFont("FreeSansBoldOblique.ttf", 30);
		if (m_Font == NULL) {
			LOG_ERROR("Unable to open font. SDL error: {}");
			return false;
		}
		// Init color for text
		m_textColor = { 255, 255, 255 };
		
		
		return true;
	}

	bool Renderer::Shutdown()
	{
		LOG_INFO("Shutting down Renderer");

		if (m_NativeRenderer != nullptr)
		{
			SDL_DestroyRenderer(m_NativeRenderer);
		}

		m_NativeRenderer = nullptr;

		m_Window.reset();

		return true;
	}

	vec2 GetScreenPosition(vec2 targetPosition, const Entity* camera)
	{
		vec2 screenCenter{ camera->GetComponent<TransformComponent>()->m_Size / 2.0f };
		return targetPosition - camera->GetComponent<TransformComponent>()->m_Position + screenCenter;
	}

	bool IsInsideScreen(vec2 targetWorldPosition, vec2 targetSize, const Entity* camera)
	{
		vec2 screenPosition = GetScreenPosition(targetWorldPosition, camera);
		return (screenPosition.x + targetSize.x / 2.0f >= 0 && screenPosition.x - targetSize.x / 2.0f <= camera->GetComponent<TransformComponent>()->m_Size.x)
			&& (screenPosition.y + targetSize.y / 2.0f >= 0 && screenPosition.y - targetSize.y / 2.0f <= camera->GetComponent<TransformComponent>()->m_Size.y);
	}

	void Renderer::DrawEntities(const std::vector<Entity*>& renderables_, const Entity* camera)
	{
		for (const auto r : renderables_)
		{
			DrawEntity(r, camera);
		}
	}

	void Renderer::DrawEntity(const Entity* r, const Entity* camera)
	{

		auto transform = r->GetComponent<TransformComponent>();
		auto sprite = r->GetComponent<SpriteComponent>();

		vec2 size = transform->m_Size;
		if (size == vec2{ 0.f, 0.f }) // Use size of texture if size of entity isn't set
		{
			int w, h;
			SDL_QueryTexture(sprite->m_Image->m_Texture, NULL, NULL, &w, &h);
			size.x = static_cast<float>(w);
			size.y = static_cast<float>(h);
		}

		if (IsInsideScreen(transform->m_Position, vec2(size.x, size.y), camera))
		{
			vec2 screenPosition = GetScreenPosition(transform->m_Position, camera);
			auto cameraSize = camera->GetComponent<Engine::TransformComponent>()->m_Size;
			float scaleX = (float)m_Window->GetWindowData().m_Width / (float)cameraSize.x;
			float scaleY = (float)m_Window->GetWindowData().m_Height / (float)cameraSize.y;
;			SDL_Rect dst{ (int)((screenPosition.x - size.x / 2) * scaleX), (int)((screenPosition.y - size.y / 2)*scaleY), (int)(size.x * scaleX), (int)(size.y*scaleY) };
			SDL_RendererFlip flip = static_cast<SDL_RendererFlip>((sprite->m_FlipHorizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (sprite->m_FlipVertical ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE));

			SDL_RenderCopyEx(
				m_NativeRenderer,
				sprite->m_Image->m_Texture,
				NULL,
				&dst,
				transform->m_Rotation,
				NULL,
				flip);

#ifdef _DEBUG
			// DebugDraw
			SDL_SetRenderDrawColor(m_NativeRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

			SDL_RenderDrawPoint(m_NativeRenderer, (int)screenPosition.x, (int)screenPosition.y);

			if (auto collider = r->GetComponent<CollisionComponent>())
			{
				SDL_SetRenderDrawColor(m_NativeRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
				dst = { (int)((screenPosition.x - collider->m_Size.x / 2)*scaleX), (int)((screenPosition.y - collider->m_Size.y / 2)*scaleY), (int)(collider->m_Size.x*scaleX), (int)(collider->m_Size.y*scaleY) };
				SDL_RenderDrawRect(m_NativeRenderer, &dst);

			}

			SetBackgroundColor(m_BackgroundColor);
#endif

		}


	}

	void Renderer::ShowTexts(const std::vector<Entity*>& texts_, const Entity* camera) {

		for (const auto r : texts_)
		{
			ShowText(r, camera);
		}

	}
	void Renderer::ShowText(const Entity* text_, const Entity* camera) {

		auto transform = text_->GetComponent<TransformComponent>();
		auto text = text_->GetComponent<TextComponent>();

		SDL_Rect rect;

		int text_width;
		int text_height;
		SDL_Surface* surface;

		surface = TTF_RenderText_Solid(m_Font, text->m_text.c_str(), m_textColor);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(m_NativeRenderer, surface);
		text_width = surface->w;
		text_height = surface->h;

		SDL_FreeSurface(surface);

		rect.x = (int)transform->m_Position.x;
		rect.y = (int)transform->m_Position.y;
		rect.w = (int)text_width;
		rect.h = (int)text_height;

		SDL_RenderCopy(m_NativeRenderer, texture, NULL, &rect);
		SDL_DestroyTexture(texture);
	}

	void Renderer::SetBackgroundColor(unsigned char bgR_, unsigned char bgG_, unsigned char bgB_, unsigned char bgA_)
	{
		m_BackgroundColor.r = bgR_;
		m_BackgroundColor.g = bgG_;
		m_BackgroundColor.b = bgB_;
		m_BackgroundColor.a = bgA_;
		SDL_SetRenderDrawColor(m_NativeRenderer, bgR_, bgG_, bgB_, bgA_);
	}

	void Renderer::SetBackgroundColor(const Color& col_)
	{
		m_BackgroundColor = col_;
		SDL_SetRenderDrawColor(m_NativeRenderer, m_BackgroundColor.r, m_BackgroundColor.g, m_BackgroundColor.b, m_BackgroundColor.a);
	}

	void Renderer::BeginScene() const
	{
		SDL_RenderClear(m_NativeRenderer);
	}

	void Renderer::EndScene() const
	{
		SDL_RenderPresent(m_NativeRenderer);
	}

	int Renderer::MaxTextureWidth() const
	{
		return m_RendererInfo.max_texture_width;
	}

	int Renderer::MaxTextureHeight() const
	{
		return m_RendererInfo.max_texture_height;
		
	}

	Renderer::~Renderer()
	{
		Shutdown();
	}

}
