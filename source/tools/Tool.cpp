#include "Tool.h"

#include "core/resource.h"
#include "helpers/image.h"


const float Tool::HANDLE_SIZE = 15.0f;
const float Tool::SELECTION_RANGE_SQUARED = 100.0f;

Tool::Handle Tool::RESIZE_TOP_LEFT     = { nullptr, nullptr };
Tool::Handle Tool::RESIZE_TOP_RIGHT    = { nullptr, nullptr };
Tool::Handle Tool::RESIZE_HORIZONTAL   = { nullptr, nullptr };
Tool::Handle Tool::RESIZE_VERTICAL     = { nullptr, nullptr };

Tool::Handle Tool::ROTATE_TOP_LEFT     = { nullptr, nullptr };
Tool::Handle Tool::ROTATE_TOP_RIGHT    = { nullptr, nullptr };
Tool::Handle Tool::ROTATE_BOTTOM_LEFT  = { nullptr, nullptr };
Tool::Handle Tool::ROTATE_BOTTOM_RIGHT = { nullptr, nullptr };

Tool::Handle Tool::HANDLE_CIRCLE       = { nullptr, nullptr };
Tool::Handle Tool::HANDLE_SQUARE       = { nullptr, nullptr };
Tool::Handle Tool::HANDLE_DIAMOND      = { nullptr, nullptr };
Tool::Handle Tool::HANDLE_MOVE         = { nullptr, nullptr };

HCURSOR Tool::CURSOR_SELECT            = nullptr;
HCURSOR Tool::CURSOR_SELECT_HIGHLIGHT  = nullptr;
HCURSOR Tool::CURSOR_PEN               = nullptr;
HCURSOR Tool::cursor = CURSOR_SELECT;

bool Tool::resources_loaded = false;

void Tool::set_application_cursor() {
    SetCursor(cursor);
}

Tool::Tool(Drawing& drawing, Defaults& defaults, HCURSOR cursor) : drawing(drawing), defaults(defaults) {
    // Don't load handle images until we construct the first tool, which should be after Gdiplus has started up. See class Initializer for details.
    if (!resources_loaded) {
        HINSTANCE hInstance = GetModuleHandle(nullptr);

        RESIZE_TOP_LEFT     = { image_from_resource(hInstance, IDB_RESIZE_TOP_LEFT, L"PNG"),     image_from_resource(hInstance, IDB_RESIZE_TOP_LEFT_SELECTED, L"PNG") };
        RESIZE_TOP_RIGHT    = { image_from_resource(hInstance, IDB_RESIZE_TOP_RIGHT, L"PNG"),    image_from_resource(hInstance, IDB_RESIZE_TOP_RIGHT_SELECTED, L"PNG") };
        RESIZE_HORIZONTAL   = { image_from_resource(hInstance, IDB_RESIZE_HORIZONTAL, L"PNG"),   image_from_resource(hInstance, IDB_RESIZE_HORIZONTAL_SELECTED, L"PNG") };
        RESIZE_VERTICAL     = { image_from_resource(hInstance, IDB_RESIZE_VERTICAL, L"PNG"),     image_from_resource(hInstance, IDB_RESIZE_VERTICAL_SELECTED, L"PNG") };

        ROTATE_TOP_LEFT     = { image_from_resource(hInstance, IDB_ROTATE_TOP_LEFT, L"PNG"),     image_from_resource(hInstance, IDB_ROTATE_TOP_LEFT_SELECTED, L"PNG") };
        ROTATE_TOP_RIGHT    = { image_from_resource(hInstance, IDB_ROTATE_TOP_RIGHT, L"PNG"),    image_from_resource(hInstance, IDB_ROTATE_TOP_RIGHT_SELECTED, L"PNG") };
        ROTATE_BOTTOM_LEFT  = { image_from_resource(hInstance, IDB_ROTATE_BOTTOM_LEFT, L"PNG"),  image_from_resource(hInstance, IDB_ROTATE_BOTTOM_LEFT_SELECTED, L"PNG") };
        ROTATE_BOTTOM_RIGHT = { image_from_resource(hInstance, IDB_ROTATE_BOTTOM_RIGHT, L"PNG"), image_from_resource(hInstance, IDB_ROTATE_BOTTOM_RIGHT_SELECTED, L"PNG") };
    
        HANDLE_CIRCLE       = { image_from_resource(hInstance, IDB_HANDLE_CIRCLE, L"PNG"),       image_from_resource(hInstance, IDB_HANDLE_CIRCLE_SELECTED, L"PNG") };
        HANDLE_SQUARE       = { image_from_resource(hInstance, IDB_HANDLE_SQUARE, L"PNG"),       image_from_resource(hInstance, IDB_HANDLE_SQUARE_SELECTED, L"PNG") };
        HANDLE_DIAMOND      = { image_from_resource(hInstance, IDB_HANDLE_DIAMOND, L"PNG"),      image_from_resource(hInstance, IDB_HANDLE_DIAMOND_SELECTED, L"PNG") };
        HANDLE_MOVE         = { image_from_resource(hInstance, IDB_HANDLE_MOVE, L"PNG"),         image_from_resource(hInstance, IDB_HANDLE_MOVE_SELECTED, L"PNG") };

        CURSOR_SELECT           = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_SELECT));
        CURSOR_SELECT_HIGHLIGHT = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_SELECT_HIGHLIGHT));
        CURSOR_PEN              = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_PEN));

        resources_loaded = true;

        set_cursor(CURSOR_SELECT);
    }
    else
        set_cursor(cursor);

    set_application_cursor();
}

std::optional<std::string> Tool::try_select_handle(Gdiplus::PointF cursor_pos, float scale) {
    auto handles = get_handles(scale);
    float closest_distance_squared = (std::numeric_limits<float>::max)();   // parentheses prevent the max macro from expanding
    std::optional<std::string> selected_handle;

    for (auto& [name, pair] : handles) {
        auto& [handle, position] = pair;
        // distance from cursor to center of handle (squared)
        float handle_distance_squared = distance_squared(cursor_pos, position, scale);

        if (handle_distance_squared < SELECTION_RANGE_SQUARED) {
            if (handle_distance_squared < closest_distance_squared) {
                closest_distance_squared = handle_distance_squared;
                selected_handle = name;
            }
        }
    }
    return selected_handle;
}

void Tool::draw_handles(Gdiplus::Graphics* graphics, std::optional<std::string> active_handle, Gdiplus::PointF cursor_pos, float scale) {
    float SIZE = HANDLE_SIZE / scale;
    HandleMap handles = get_handles(scale);
    
    // If no handle is currently being dragged, find the closest handle within range of the cursor, if any.
    if (!active_handle)
        active_handle = try_select_handle(cursor_pos, scale);

    // Draw each handle with its normal image
    for (auto& [_, pair] : handles) {
        auto& [handle, position] = pair;
        graphics->DrawImage(handle.default_image, position.X - SIZE / 2, position.Y - SIZE / 2, SIZE, SIZE);
    }

    // Draw selected handle on top
    if (active_handle) {
        auto& pair = handles[active_handle.value()];
        Gdiplus::Bitmap* selected_image = pair.first.selected_image;
        Gdiplus::PointF& position = pair.second;
        graphics->DrawImage(selected_image, position.X - SIZE / 2, position.Y - SIZE / 2, SIZE, SIZE);
    }
}

float Tool::distance_squared(Gdiplus::PointF& point_a, Gdiplus::PointF& point_b, float scale) {
    return powf((point_a.X - point_b.X) * scale, 2.0f) + powf((point_a.Y - point_b.Y) * scale, 2.0f);
}
