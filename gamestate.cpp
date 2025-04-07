#include "gamestate.h"

void Gamestate::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "failed in SDL_Init(): " << SDL_GetError() << endl;
	}
	else
	{
		window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (!window)
		{
			cout << "failed to create window \n" << SDL_GetError() << endl;
		}
		else
		{
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (!renderer)
			{
				cout << "failed to create renderer\n" << SDL_GetError() << endl;
			}
		}
	}
	
	if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG)))
	{
		cout << "khong the khoi tao SDL_image: " << IMG_GetError() << endl;
	}

	if (TTF_Init() < 0) cout << "khong the khoi tao ttf" << endl;
	
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) cout << "Failed to initialize SDL_mixer: " << Mix_GetError() << endl;
	
	// load ảnh background, bóng, thanh trượt
	background = load_texture("image/background.jpg");
	if (!background) cout << "khong the tai anh nen" << endl;
	
	menu_background = load_texture("image/menu_background.jpg");
	if (!menu_background) cout << "khong the tai menu_background" << endl;

	img_ball = load_texture("image/ball.png");
	if (!img_ball) cout << "khong the tai hinh anh bong" << endl;
	
	img_paddle = load_texture("image/paddle.png");
	if (!img_paddle) cout << "khong the tai hinh anh thanh truot" << endl;
	
	win_screen = load_texture("image/win_screen.png");
	if (!win_screen) cout << "khong the tai hinh anh win_screen" << endl;

	lose_screen = load_texture("image/lose_screen.png");
	if (!lose_screen) cout << "khong the tai hinh anh lose_screen" << endl;

	brick1 = load_texture("image/brick1.jpg");
	if (!brick1) cout << "khong the tai hinh anh brick1" << endl;

	brick2 = load_texture("image/brick2.jpg");
	if (!brick2) cout << "khong the tai hinh anh brick2" << endl;

	// tải font chữ
	font = TTF_OpenFont("font/SVN-Coder's Crux.otf", 28);
	if (!font) cout << "khong the tai font chu" << endl;

	// load tiếng khi bóng chạm vào gạch và thanh trượt
	sound_hit = Mix_LoadWAV("music/sound_hit.mp3");
	if (!sound_hit) cout << "khong the tai sound_hit" << endl;
	
	sound_menu = Mix_LoadWAV("music/menu_sound.mp3");
	if (!sound_menu) cout << "khong the tai sound_menu" << endl;
	
	sound_background = Mix_LoadMUS("music/sound_background.mp3");
	if (!sound_background) cout << "khong the tai sound back_ground" << endl;

	win_sound = Mix_LoadMUS("music/win_sound.mp3");
	if (!win_sound) cout << "khong the tai wiin_sound" << endl;

	lose_sound = Mix_LoadMUS("music/lose_sound.mp3");
	if (!lose_sound) cout << "khong the tai lose_sound" << endl;

	Mix_PlayMusic(sound_background, -1);

}

SDL_Texture* Gamestate::load_texture(const string& path)
{
	SDL_Texture* new_texture = nullptr;
	SDL_Surface* loaded_surface = IMG_Load(path.c_str());
	if (!loaded_surface)
	{
		cout << "khong the load file" << endl;
	}
	else
	{
		new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
		if (!new_texture)
		{
			cout << "khong the khoi tao new_texture" << endl;
		}
		SDL_FreeSurface(loaded_surface);
	}
	return new_texture;
}

bool Gamestate::show_menu()
{
	bool in_menu = true;

	SDL_Event event;

	while (in_menu)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) return false;
			
			int x, y;
			SDL_GetMouseState(&x, &y);
		
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (x >= play_button.x && x <= play_button.x + play_button.w && y >= play_button.y && y <= play_button.y + play_button.h) {
					Mix_PlayChannel(-1, sound_menu, 0);
					return true;
				}
				if (x >= exit_button.x && x <= exit_button.x + exit_button.w && y >= exit_button.y && y <= exit_button.y + exit_button.h) {
					Mix_PlayChannel(-1, sound_menu, 0);
					return false;
				}
			}
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, menu_background, nullptr, nullptr);

		SDL_Texture* play_text = render_text("PLAY", color);
		SDL_Texture* exit_text = render_text("EXIT", color);

		SDL_RenderCopy(renderer, play_text, nullptr, &play_button);
		SDL_RenderCopy(renderer, exit_text, nullptr, &exit_button);

		SDL_DestroyTexture(play_text);
		SDL_DestroyTexture(exit_text);

		SDL_RenderPresent(renderer);
	}
	return false;
}

void Gamestate::show_end_screen(bool win)
{
	SDL_RenderClear(renderer);
	if (win)
	{
		SDL_RenderCopy(renderer, win_screen, nullptr, nullptr);
		Mix_PlayMusic(win_sound, 0);
	}
	else
	{
		SDL_RenderCopy(renderer, lose_screen, nullptr, nullptr);
		Mix_PlayMusic(lose_sound, 0);
	}

	// Tạo nút "TRY AGAIN" và "EXIT"
	SDL_Texture* try_again_text = render_text("TRY AGAIN", color);
	SDL_Texture* exit_text = render_text("EXIT", color);

	SDL_RenderCopy(renderer, try_again_text, nullptr, &try_again_button);
	SDL_RenderCopy(renderer, exit_text, nullptr, &exit_button2);

	SDL_DestroyTexture(try_again_text);
	SDL_DestroyTexture(exit_text);

	SDL_RenderPresent(renderer);

	// Chờ người chơi chọn
	bool wait = true;
	while (wait)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
				return;
			}
			
			int x, y;
			SDL_GetMouseState(&x, &y);

			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (x >= try_again_button.x && x <= try_again_button.x + try_again_button.w && y >= try_again_button.y && y <= try_again_button.y + try_again_button.h)
				{
					Mix_PlayChannel(-1, sound_menu, 0);
					reset_bricks();
					wait = false;
					wait_key_down = true;
				}

				if (x >= exit_button2.x && x <= exit_button2.x + exit_button2.w && y >= exit_button2.y && y <= exit_button2.y + exit_button2.h)
				{
					Mix_PlayChannel(-1, sound_menu, 0);
					running = false;
					wait = false;
				}
			}
		}
	}
}


SDL_Texture* Gamestate::render_text(const string& text, SDL_Color color)
{
	SDL_Surface* text_surface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (!text_surface)
	{
		cout << "khong the load text_surface" << endl;
		return nullptr;
	}

	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_FreeSurface(text_surface);

	if (!text_texture)
	{
		cout << "khong the load text_texture" << endl;
		return nullptr;
	}

	return text_texture;
}

Gamestate::Gamestate()
{
	window = nullptr;
	renderer = nullptr;

	background = nullptr;
	menu_background = nullptr;
	img_ball = nullptr;
	img_paddle = nullptr;
	brick1 = nullptr;
	brick2 = nullptr;
	win_screen = nullptr;
	lose_screen = nullptr;

	font = nullptr;
	
	sound_hit = nullptr;
	sound_menu = nullptr;
	sound_background = nullptr;
	win_sound = nullptr;
	lose_sound = nullptr;

	running = true;
	wait_key_down = true;

	// vị trí nút play, exit, try again, exit
	play_button = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 40, 100, 30 };
	exit_button = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, 100, 30 };
	try_again_button = { SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT - 120, 160, 40 };
	exit_button2 = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT - 60, 100, 40 };
	// vị trí hiển thị số lượng mạng còn lại
	live_left = { 0, SCREEN_HEIGHT - 50, 60, 50 };

	color = { 255, 255, 255, 255 };

	// thiết lập vị trí ban đầu cho paddle
	paddle.w = SCREEN_WIDTH / 4;
	paddle.h = paddle.w / 4;
	paddle.x = SCREEN_WIDTH / 2 - paddle.w / 2;
	paddle.y = SCREEN_HEIGHT - paddle.h;

	// thiết lập vị trí ban đầu cho quả bóng
	ball.x = paddle.x + paddle.w / 2;
	ball.y = paddle.y - BALL_SIZE;
	ball.w = BALL_SIZE;
	ball.h = BALL_SIZE;

	brick.x = brick.y = 0;
	brick.w = brick.h = SPACING;

	vX = BALL_SPEED;
	vY = BALL_SPEED;

	// thiết lập lại tất cả viên gạch
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			bricks[i][j] = true;
		}
	}

	live_count = 3;
}

void Gamestate::reset_bricks()
{
	// thiết lập lại vị trí ban đầu cho paddle
	paddle.w = SCREEN_WIDTH / 4;
	paddle.h = paddle.w / 4;
	paddle.x = SCREEN_WIDTH / 2 - paddle.w / 2;
	paddle.y = SCREEN_HEIGHT - paddle.h;

	// thiết lập lại vị trí ban đầu cho quả bóng
	ball.x = paddle.x + paddle.w / 2;
	ball.y = paddle.y - BALL_SIZE;
	ball.w = BALL_SIZE;
	ball.h = BALL_SIZE;

	brick.x = brick.y = 0;
	brick.w = brick.h = SPACING;

	// thiết lập lại tất cả viên gạch
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			bricks[i][j] = true;
		}
	}

	// đặt lại số mạng
	live_count = 3;

	// cho chạy lại nhạc nền
	Mix_PlayMusic(sound_background, -1);
}

void Gamestate::set_bricks(int row, int col)
{
	brick.x = ((col + 1) * SPACING) + (col * brick.w) - (SPACING / 2);
	brick.y = brick.h * 3 + ((row + 1) * SPACING) + (row * brick.h) - (SPACING / 2);
}


void Gamestate::update()
{
	if (wait_key_down) return; // nếu chưa có phím nào được nhấn để bắt đầu trò chơi thì dừng lại

	// di chuyển quả bóng theo phương ngang và dọc
	ball.x += vX;
	ball.y += vY;

	// nếu va chạm giữa bóng và thanh trượt thì cho bóng bay ngược lại lên trên
	if (SDL_HasIntersection(&ball, &paddle)) 
	{
		vY = -vY;
	}

	// nếu quả bóng đập vào thanh màn hình ở trên thì cho bay ngược lại theo chiều dọc
	if (ball.y <= 0)
	{
		vY = -vY;
	}

	// nếu bóng chạm vào thanh bên trái hoặc bên phải thì cho bay ngược lại theo chiều ngang
	if (ball.x <= 0 || ball.x + BALL_SIZE >= SCREEN_WIDTH)
	{
		vX = -vX;
	}

	// nếu bóng rơi ra khỏi màn hình 
	if (ball.y + BALL_SIZE >= SCREEN_HEIGHT)
	{
		vY = -vY;
		live_count--;
	}

	// kiểm tra nếu paddle ra khỏi màn hình thì giữ nguyên
	if (paddle.x <= 0) paddle.x = 0;
	if (paddle.x + paddle.w >= SCREEN_WIDTH) paddle.x = SCREEN_WIDTH - paddle.w;

	// Xử lý va chạm với gạch

	bool reset = true;

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			set_bricks(i, j);
			if (SDL_HasIntersection(&ball, &brick) && bricks[i][j])
			{
				Mix_PlayChannel(-1, sound_hit, 0);
				bricks[i][j] = false;
				// Va chạm từ trên hoặc dưới
				if (ball.y + BALL_SIZE - vY <= brick.y || ball.y - vY >= brick.y + brick.h)
					vY = -vY;  
				else
					// Va chạm từ hai bên
					vX = -vX;  
			}

			if (bricks[i][j]) reset = false;
		}
	}

	if (reset)
	{
		show_end_screen(true);
	}

	if (live_count <= 0)
	{
		show_end_screen(false);
	}
}


void Gamestate::input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) running = false;
		if (event.type == SDL_KEYDOWN) wait_key_down = false;
	}
	const Uint8* keystates = SDL_GetKeyboardState(NULL);
	if (keystates[SDL_SCANCODE_ESCAPE]) running = false;
	if (keystates[SDL_SCANCODE_LEFT]) paddle.x -= SPEED; // di chuyển sang trái
	if (keystates[SDL_SCANCODE_RIGHT]) paddle.x += SPEED; // di chuyển sang phải

}

void Gamestate::render()
{
	SDL_Delay(3);

	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, background, nullptr, nullptr);

	SDL_Texture* live_text = render_text("LIVES: " + to_string(live_count), color);
	if (!live_text) cout << "khong the tai lives_text" << endl;

	SDL_RenderCopy(renderer, live_text, nullptr, &live_left);
	SDL_DestroyTexture(live_text);
	
	SDL_RenderCopy(renderer, img_ball, nullptr, &ball);
	
	SDL_RenderCopy(renderer, img_paddle, nullptr, &paddle);

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (bricks[i][j])
			{
				set_bricks(i, j);
				if (i % 2 == 0)
				{
					SDL_RenderCopy(renderer, brick1, nullptr, &brick);
				}
				else
				{
					SDL_RenderCopy(renderer, brick2, nullptr, &brick);
				}
			}
		}
	}

	// Hiển thị tất cả hình vẽ vừa renderer lên màn hình
	SDL_RenderPresent(renderer);

}


void Gamestate::cleanup()
{
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	SDL_DestroyWindow(window);
	window = nullptr;

	SDL_DestroyTexture(background);
	background = nullptr;

	SDL_DestroyTexture(menu_background);
	menu_background = nullptr;

	SDL_DestroyTexture(img_ball);
	img_ball = nullptr;

	SDL_DestroyTexture(img_paddle);
	img_paddle = nullptr;

	SDL_DestroyTexture(brick1);
	brick1 = nullptr;

	SDL_DestroyTexture(brick2);
	brick2 = nullptr;

	SDL_DestroyTexture(win_screen);
	win_screen = nullptr;

	SDL_DestroyTexture(lose_screen);
	lose_screen = nullptr;

	TTF_CloseFont(font);
	font = nullptr;

	Mix_FreeChunk(sound_hit);
	sound_hit = nullptr;

	Mix_FreeChunk(sound_menu);
	sound_menu = nullptr;

	Mix_FreeMusic(sound_background);
	sound_background = nullptr;

	Mix_FreeMusic(win_sound);
	win_sound = nullptr;

	Mix_FreeMusic(lose_sound);
	lose_sound = nullptr;

	Mix_CloseAudio();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}




