#include "StdAfx.h"
#include "CustomDragHandler.h"

namespace JGW
{
    CustomDragHandler::CustomDragHandler(void)
    {
    }


    CustomDragHandler::~CustomDragHandler(void)
    {
    }

    bool CustomDragHandler::OnDragEnter(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> dragData, DragOperationsMask mask)
    {
        // 示例：仅允许复制操作
        return (mask & DRAG_OPERATION_COPY) ? false : true;
    }

    void CustomDragHandler::OnDraggableRegionsChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const std::vector<CefDraggableRegion>& regions)
    {

    }
}
