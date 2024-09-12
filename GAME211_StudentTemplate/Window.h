
class Window {
private:
	struct SDL_Window* window;   
	struct SDL_Surface* screenSurface;
    struct SDL_Renderer* renderer;
	int width, height;
	
public:
	Window(int width_, int height_ );
	~Window();
	bool OnCreate();
	void OnDestroy();
	SDL_Window* GetSDL_Window();
};

