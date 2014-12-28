/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 21/02/14
 * Time: 20:46
 * To change this template use File | Settings | File Templates.
 */
goog.provide('dotasim.assets.data.GameSettings');

dotasim.GameSettings = {
    GameHud: {
        SHOW_HIDE_ANIMATION_TIME: 1000,
        SidePanel: {
            WIDTH: 100,
            HEIGHT: 480,
            DeployGrid: {
                WIDTH: 100,
                HEIGHT: 146,
                GAP_Y: 21,
                BORDER_SIZE: 8
            }
        },
        CenterPanel: {
            X: 120,
            Y: 120,
            WIDTH: 400,
            HEIGHT: 300,
            BORDER: 2,
            PlayerGoldDisplay: {
                WIDTH: 100,
                HEIGHT: 25,
                BORDER: 3,
                SYMBOL: {
                    SIZE: 20,
                    BORDER: 1
                }
            }
        }
    },
    POS: {
        HQ: {
            0: {x:30, y:240},
            1: {x:610, y:240}
        },
        TOWER: {

        }
    }
};

dotasim.GamePlayData = {
    GENERAL: {
        HERO_RETRACT_TIME: 2500,        //in ms
        LANE_MAX_HERO: 3,
        MOVE_ATTACK_SCAN_RANGE: 70,
        EXP_RECEIVE_RANGE: 100,
        CREEP_SPAWN_CD: 10000,          //in ms
        HERO_REBORN_TIME: 10000         //in ms
    },
    HQ: {
        HP: 1000
    },
    TOWER: {
        HP: 120,
        ATK: 2,
        ATK_RANGE: 50,
        ATK_CD: 1000
    },
    CREEP: {
        HP: 10,
        ATK: 1,
        ATK_RANGE: 15,
        ATK_CD: 1500,
        SPD: 0.5
    },
    RANGE_CREEP: {
        HP: 4,
        ATK: 1,
        ATK_RANGE: 50,
        ATK_CD: 1500,
        SPD: 0.5
    },
    HERO: {
        Aryan:{
            HP: 21,
            ATK: 2,
            ATK_RANGE: 18,
            ATK_CD: 1500,
            SPD: 0.5
        },
        Angi:{
            HP: 21,
            ATK: 1,
            ATK_RANGE: 50,
            ATK_CD: 1500,
            SPD: 0.5
        }
    }
};

dotasim.GameCoreData = {
    MAP_GRID_CELL_SIZE: 4
};
