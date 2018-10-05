/*
 * Copyright (c) 2018 BestSolution and Others. All rights reserved.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v. 2.0, which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception, which is available at
 * https://www.gnu.org/software/classpath/license.html.
 *
 * SPDX-License-Identifier: EPL-2.0 OR GPL-2.0 WITH Classpath-exception-2.0
 */
#ifndef INCLUDE_MOUSEEVENT_H_
#define INCLUDE_MOUSEEVENT_H_

namespace org {
namespace eclipse {
namespace efxclipse {
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
}


#endif
