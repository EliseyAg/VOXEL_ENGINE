#pragma once
#include "Keys.hpp"

#include <functional>
#include <array>

namespace Events
{
    enum class EventType
    {
        WindowResize = 0,
        WindowClose,

        KeyPressed,
        KeyReleased,

        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,


        EventsCount
    };

    struct BaseEvent
    {
        virtual ~BaseEvent() = default;
        virtual EventType get_type() const = 0;
    };


    class EventDispatcher
    {
    public:
        template<typename EventType>
        void add_event_listener(std::function<void(EventType&)> callback)
        {
            auto baseCallback = [func = std::move(callback)](BaseEvent& e)
            {
                if (e.get_type() == EventType::type)
                {
                    func(static_cast<EventType&>(e));
                }
            };
            m_eventCallbacks[static_cast<size_t>(EventType::type)] = std::move(baseCallback);
        }

        void dispatch(BaseEvent& event)
        {
            auto& callback = m_eventCallbacks[static_cast<size_t>(event.get_type())];
            if (callback)
            {
                callback(event);
            }
        }

    private:
        std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventsCount)> m_eventCallbacks;
    };


    struct EventMouseMoved : public BaseEvent
    {
        EventMouseMoved(const double new_x, const double new_y)
            : x(new_x)
            , y(new_y)
        {
        }

        virtual EventType get_type() const override
        {
            return type;
        }

        double x;
        double y;

        static const EventType type = EventType::MouseMoved;
    };

    struct EventWindowResize : public BaseEvent
    {
        EventWindowResize(const unsigned int new_width, const unsigned int new_height)
            : width(new_width)
            , height(new_height)
        {
        }

        virtual EventType get_type() const override
        {
            return type;
        }

        unsigned int width;
        unsigned int height;

        static const EventType type = EventType::WindowResize;
    };

    struct EventWindowClose : public BaseEvent
    {
        virtual EventType get_type() const override
        {
            return type;
        }

        static const EventType type = EventType::WindowClose;
    };

    struct EventKeyPressed : public BaseEvent
    {
        EventKeyPressed(const KeyCode key_code, const bool repeated)
            : key_code(key_code)
            , repeated(repeated)
        {
        }

        virtual EventType get_type() const override
        {
            return type;
        }

        KeyCode key_code;
        bool repeated;

        static const EventType type = EventType::KeyPressed;
    };

    struct EventKeyReleased : public BaseEvent
    {
        EventKeyReleased(const KeyCode key_code)
            : key_code(key_code)
        {
        }

        virtual EventType get_type() const override
        {
            return type;
        }

        KeyCode key_code;

        static const EventType type = EventType::KeyReleased;
    };

    struct EventMouseButtonPressed : public BaseEvent
    {
        EventMouseButtonPressed(const MouseButton mouse_button, const double x_pos, const double y_pos)
            : mouse_button(mouse_button)
            , x_pos(x_pos)
            , y_pos(y_pos)
        {
        }

        virtual EventType get_type() const override
        {
            return type;
        }

        MouseButton mouse_button;
        double x_pos;
        double y_pos;

        static const EventType type = EventType::MouseButtonPressed;
    };

    struct EventMouseButtonReleased : public BaseEvent
    {
        EventMouseButtonReleased(const MouseButton mouse_button, const double x_pos, const double y_pos)
            : mouse_button(mouse_button)
            , x_pos(x_pos)
            , y_pos(y_pos)
        {
        }

        virtual EventType get_type() const override
        {
            return type;
        }

        MouseButton mouse_button;
        double x_pos;
        double y_pos;

        static const EventType type = EventType::MouseButtonReleased;
    };
}