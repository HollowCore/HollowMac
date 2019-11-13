//
//  HCMenu.c
//  Test
//
//  Created by Matt Stoker on 10/31/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowMac.h"

CTEST(HCMenu, Creation) {
    HCMenuRef menu = HCMenuCreate();
    HCRelease(menu);
}

CTEST(HCMenu, EqualHash) {
    HCMenuRef a = HCMenuCreate();
    HCMenuRef b = HCMenuCreate();
    ASSERT_FALSE(HCIsEqual(a, b));
    ASSERT_EQUAL(HCHashValue(a), HCHashValue(a));
    ASSERT_EQUAL(HCHashValue(b), HCHashValue(b));
    HCRelease(a);
    HCRelease(b);
}

CTEST(HCMenu, Print) {
    HCMenuRef menu = HCMenuCreate();
    HCMenuPrint(menu, stdout); // TODO: Not to stdout
    HCRelease(menu);
}

CTEST(HCMenu, Title) {
    HCMenuRef menu = HCMenuCreate();
    HCStringRef title = HCStringCreateWithCString("Great Stuff");
    HCMenuSetTitle(menu, title);
    HCStringRef titleRetreived = HCMenuTitleRetained(menu);
    ASSERT_TRUE(HCIsEqual(title, titleRetreived));
    HCRelease(title);
    HCRelease(titleRetreived);
    HCRelease(menu);
}

CTEST(HCMenu, Shortcut) {
    HCMenuRef menu = HCMenuCreate();
    HCStringRef shortcutKey = HCStringCreateWithCString("q");
    HCMenuSetShortcutKey(menu, shortcutKey);
    HCStringRef shortcutKeyRetreived = HCMenuShortcutKeyRetained(menu);
    ASSERT_TRUE(HCIsEqual(shortcutKey, shortcutKeyRetreived));
    HCRelease(shortcutKey);
    HCRelease(shortcutKeyRetreived);
    HCRelease(menu);
}

void HCMenuTestClick(void* context, HCMenuRef menu) {
    ASSERT_NOT_NULL(menu);
    ASSERT_TRUE(context == (void*)0xDEADBEEF);
}

CTEST(HCMenu, Click) {
    HCMenuRef menu = HCMenuCreate();
    HCMenuSetClickCallback(menu, HCMenuTestClick, (void*)0xDEADBEEF);
    ASSERT_TRUE(HCMenuClickCallback(menu) == HCMenuTestClick);
    HCMenuPerformClick(menu);
    HCRelease(menu);
}

CTEST(HCMenu, RelatedMenus) {
    HCMenuRef menu = HCMenuCreate();
    ASSERT_TRUE(HCMenuChildMenuCount(menu) == 0);
    
    HCMenuRef leftChildMenu = HCMenuCreate();
    HCMenuAddChildMenu(menu, leftChildMenu);
    ASSERT_TRUE(HCMenuChildMenuCount(menu) == 1);
    ASSERT_TRUE(HCMenuChildMenuCount(leftChildMenu) == 0);
    
    HCMenuRef leftLeftChildMenu = HCMenuCreate();
    HCMenuAddChildMenu(leftChildMenu, leftLeftChildMenu);
    HCMenuRef leftMiddleChildMenu = HCMenuCreate();
    HCMenuAddChildMenu(leftChildMenu, leftMiddleChildMenu);
    HCMenuRef leftRightChildMenu = HCMenuCreate();
    HCMenuAddChildMenu(leftChildMenu, leftRightChildMenu);
    ASSERT_TRUE(HCMenuChildMenuCount(menu) == 1);
    ASSERT_TRUE(HCMenuChildMenuCount(leftChildMenu) == 3);
    
    HCMenuRef rightChildMenu = HCMenuCreate();
    HCMenuAddChildMenu(menu, rightChildMenu);
    ASSERT_TRUE(HCMenuChildMenuCount(menu) == 2);
    ASSERT_TRUE(HCMenuChildMenuCount(leftChildMenu) == 3);
    ASSERT_TRUE(HCMenuChildMenuCount(rightChildMenu) == 0);
    
    HCMenuRef leftChildMenuRetrieved = HCMenuChildMenuAtIndexRetained(menu, 0);
    ASSERT_TRUE(HCIsEqual(leftChildMenuRetrieved, leftChildMenu));
    HCMenuRef rightChildMenuRetrieved = HCMenuChildMenuAtIndexRetained(menu, 1);
    ASSERT_TRUE(HCIsEqual(rightChildMenuRetrieved, rightChildMenu));
    HCMenuRef leftLeftChildMenuRetrieved = HCMenuChildMenuAtIndexRetained(leftChildMenu, 0);
    ASSERT_TRUE(HCIsEqual(leftLeftChildMenuRetrieved, leftLeftChildMenu));
    HCMenuRef leftMiddleChildMenuRetrieved = HCMenuChildMenuAtIndexRetained(leftChildMenu, 1);
    ASSERT_TRUE(HCIsEqual(leftMiddleChildMenuRetrieved, leftMiddleChildMenu));
    HCMenuRef leftRightChildMenuRetrieved = HCMenuChildMenuAtIndexRetained(leftChildMenu, 2);
    ASSERT_TRUE(HCIsEqual(leftRightChildMenuRetrieved, leftRightChildMenu));
    
    HCMenuRef leftLeftChildMenuParent = HCMenuParentMenuRetained(leftLeftChildMenu);
    ASSERT_TRUE(HCIsEqual(leftLeftChildMenuParent, leftChildMenu));
    
    HCMenuRemoveChildMenu(leftChildMenu, 1);
    ASSERT_TRUE(HCMenuChildMenuCount(menu) == 2);
    ASSERT_TRUE(HCMenuChildMenuCount(leftChildMenu) == 2);
    ASSERT_TRUE(HCMenuChildMenuCount(rightChildMenu) == 0);
    HCMenuRef notLeftMiddleChildMenuRetrieved = HCMenuChildMenuAtIndexRetained(leftChildMenu, 1);
    ASSERT_FALSE(HCIsEqual(notLeftMiddleChildMenuRetrieved, leftMiddleChildMenu));
    
    HCMenuRemoveChildMenu(menu, 0);
    ASSERT_TRUE(HCMenuChildMenuCount(menu) == 1);
    ASSERT_TRUE(HCMenuChildMenuCount(leftChildMenu) == 2);
    ASSERT_TRUE(HCMenuChildMenuCount(rightChildMenu) == 0);
    HCMenuRef notLeftChildMenuRetrieved = HCMenuChildMenuAtIndexRetained(menu, 0);
    ASSERT_FALSE(HCIsEqual(notLeftChildMenuRetrieved, leftChildMenu));
    ASSERT_TRUE(HCMenuParentMenuRetained(leftChildMenu) == NULL);
    
    HCRelease(menu);
    HCRelease(leftChildMenu);
    HCRelease(leftLeftChildMenu);
    HCRelease(leftMiddleChildMenu);
    HCRelease(leftRightChildMenu);
    HCRelease(rightChildMenu);
    HCRelease(leftChildMenuRetrieved);
    HCRelease(rightChildMenuRetrieved);
    HCRelease(leftLeftChildMenuRetrieved);
    HCRelease(leftMiddleChildMenuRetrieved);
    HCRelease(leftRightChildMenuRetrieved);
    HCRelease(leftLeftChildMenuParent);
    HCRelease(notLeftMiddleChildMenuRetrieved);
    HCRelease(notLeftChildMenuRetrieved);
}
