#ifndef HEADER_INPUTAGENT_H
#define HEADER_INPUTAGENT_H

class KeyBinder;
class InputHandler;

#include "BaseAgent.h"
#include "Name.h"
#include "V2.h"

#include "SDL3/SDL.h"

#include <memory>

/**
 * Forward input events to handlers.
 */
class InputAgent : public BaseAgent {
    AGENT(InputAgent, Name::INPUT_NAME);
    private:
        const bool *m_keys;
        std::unique_ptr<KeyBinder> m_keyBinder;
        InputHandler *m_handler;
    private:
        V2 getMouseState(Uint32 *out_buttons);
    protected:
        virtual void own_init();
        virtual void own_update();
        virtual void own_shutdown();
    public:
        InputAgent();
        ~InputAgent();

        void installHandler(InputHandler *handler);

        KeyBinder *keyBinder() { return m_keyBinder.get(); }
};

#endif
