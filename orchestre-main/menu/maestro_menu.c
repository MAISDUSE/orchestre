#include "maestro_menu.h"

void getFeedbackString(char * response, int action, int state, int msg_size) {

    switch (action) {
        case PLAY_PAUSE:
            if (state == -1) {
                strncpy(response, "Pas d'instrument chargé", msg_size);
            } else if (state == 1) {
                strncpy(response, "Source en train de jouer", msg_size);
            } else {
                strncpy(response, "Source en pause", msg_size);
            }
            break;

        case CHG_POSITION:
            if (state == -1) {
                strncpy(response, "Pas d'instrument chargé", msg_size);
            } else {
                strncpy(response, "Position de la source mise à jour", msg_size);
            }
            break;

        case CHG_DIRECTION:
            if (state == -1) {
                strncpy(response, "Pas d'instrument chargé", msg_size);
            } else {
                strncpy(response, "Orientation de la source mise à jour", msg_size);
            }
            break;

        case CHG_INSTRUMENT:
            break;

        case QUIT:
            break;

        default:
            break;
    }
}