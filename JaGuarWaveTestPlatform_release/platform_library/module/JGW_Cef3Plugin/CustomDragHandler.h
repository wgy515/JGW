#pragma once
#include <include/cef_drag_handler.h>

namespace JGW
{
    class CustomDragHandler : public CefDragHandler
    {
    public:
        CustomDragHandler(void);
        ~CustomDragHandler(void);
    protected:
        //! 外部拖拽进入浏览器窗口时	true取消事件，false继续默认处理
        bool OnDragEnter(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> dragData, DragOperationsMask mask) override;
        //! 页面定义可拖拽区域时更新宿主窗口
        void OnDraggableRegionsChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const std::vector<CefDraggableRegion>& regions);
    public:
        IMPLEMENT_REFCOUNTING(CustomDragHandler);
    };
}
