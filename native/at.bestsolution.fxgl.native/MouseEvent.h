#ifndef INCLUDE_MOUSEEVENT_H_
#define INCLUDE_MOUSEEVENT_H_

namespace at {
namespace bestsolution {
namespace fxgl {
namespace events {

enum MouseEventType {
	PRESSED,
	RELEASED,
	CLICKED,
	MOVED,
	DRAGGED
};
enum MouseButton {
	NONE,
	PRIMARY,
	SECONDARY,
	MIDDLE
};

struct MouseEvent {
	MouseEventType type;
	MouseButton button;
	int clickCount;
	int x;
	int y;

	bool shiftDown;
	bool controlDown;
	bool metaDown;

	bool stillSincePress;
};


}
}
}
}


#endif
