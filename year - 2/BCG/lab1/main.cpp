#include <print>
#include <cstdint>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

void upscaleSurface(SDL_Surface*& source, const uint32_t scale){
   if(scale == 1)return;
   uint8_t red;
   uint8_t green;
   uint8_t blue;
   uint8_t alpha;
   SDL_Surface* result = SDL_CreateSurface(source->w * scale, source->h * scale, source->format);
   for(int32_t x = 0; x < source->w; x++){
      for(int32_t y = 0; y < source->h; y++){
	 SDL_ReadSurfacePixel(source, x, y, &red, &green, &blue, &alpha);
	 SDL_Rect scaledPixel = {x * scale, y * scale, scale, scale};
	 const SDL_PixelFormatDetails* formatDetail = SDL_GetPixelFormatDetails(source->format);
	 uint32_t color = SDL_MapRGBA(formatDetail, NULL, red, green, blue, alpha);
	 SDL_FillSurfaceRect(result, &scaledPixel, color);
      }
   }
   SDL_DestroySurface(source);
   source = result;
}

int main(){
   const uint32_t scaling = 4; 

   std::string path = static_cast<std::string>(SDL_GetBasePath()) + "test.tiff";
   SDL_IOStream* filestream = SDL_IOFromFile(path.c_str(), "r");
   SDL_Surface* testTIF = nullptr;
   testTIF = IMG_LoadTIF_IO(filestream);
   if(!testTIF){
      std::println("Can't open {}", path);
   }
   SDL_CloseIO(filestream);
   upscaleSurface(testTIF, scaling);

   SDL_Init(SDL_INIT_VIDEO);
   SDL_Window* window;
   SDL_Renderer* renderer;
   SDL_CreateWindowAndRenderer("window", testTIF->w, testTIF->h, 0, &window, &renderer);

   SDL_GetWindowSurface(window);
   SDL_UpdateWindowSurface(window);

   int32_t TIFwidth = testTIF->w;
   int32_t TIFheight = testTIF->h;
   SDL_Texture* textureTIF = SDL_CreateTextureFromSurface(renderer, testTIF);
   SDL_DestroySurface(testTIF);

   SDL_Event ev;
   bool isClosed = false;
   while(!isClosed){
      if(SDL_PollEvent(&ev)){
	 if(ev.type == SDL_EventType::SDL_EVENT_QUIT){
	    isClosed = true;
	 }
      }
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
      SDL_RenderClear(renderer);

      SDL_FRect renderRect = {0, 0, static_cast<float>(TIFwidth), static_cast<float>(TIFheight)};
      SDL_RenderTexture(renderer, textureTIF, NULL, &renderRect);

      SDL_RenderPresent(renderer);
   }
      
   SDL_DestroyWindow(window);
   SDL_Quit();
}
