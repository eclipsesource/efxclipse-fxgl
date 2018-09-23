#ifndef _INCLUDE_SHARED_MEMORY_H_
#define _INCLUDE_SHARED_MEMORY_H_

#include "Texture.h"

namespace at {
namespace bestsolution {
namespace fxgl {

struct TextureMemory {
	Texture* textures;
	int foreground;
	int background;
	int nextTexture;
	int usedTextureId;
	int renderTextureId;
};

Texture getForeground();
Texture getBackground();


Texture getNextTexture();
void renderTexture();

}
}
}


#endif
