/**
 * Created by lydashsbok on 14-7-12.
 */
goog.provide("doudizhu.features.core.l.GamePlay");

goog.require("bok.EventDispatcher");
goog.require("doudizhu.features.core.m.Game");
goog.require("doudizhu.features.core.m.Hand");
goog.require("doudizhu.features.core.l.AI");

(function() {
    doudizhu.features.core.l.GamePlay = GamePlay;
    var Game = doudizhu.features.core.m.Game;
    var Hand = doudizhu.features.core.m.Hand;
    var AI = doudizhu.features.core.l.AI;

    GamePlay.sideNames = [
        'player',
        'AI-1',
        'AI-2'
    ];

    BOK.inherits(GamePlay, EventDispatcher);
    /**
     * @constructor
     * @param {Game=} game (optional)
     * */
    function GamePlay(game) {
        EventDispatcher.call(this);

        /** @type {Array} */
        this.sideNames_ = GamePlay.sideNames;
        /** @type {doudizhu.features.core.m.Game} */
        this.game_  = game || new Game(GamePlay.sideNames);

        this.gameState_ = 'bidding';    //including: bidding, playing, finished.
        this.highBider_ = '';
        this.gameBet_ = 0;

        this.enabledAI_ = {
            0: false,
            1: true,
            2: true
        };

        this.AI_ = new AI();
    }

    /**
     * @param {Array=} sideNames
     * */
    GamePlay.prototype.newGame = function(sideNames) {
        this.sideNames_ = sideNames;
        this.game_ = new Game(this.sideNames_);
        this.gameState_ = 'bidding';
        this.highBider_ = '';
        this.gameBet_ = 0;
    };

    GamePlay.prototype.getPlayerNames = function() {
        return this.sideNames_;
    };

    GamePlay.prototype.nextBidForBanker = function(lastCallerPassed) {
        var bidderName = this.sideNames_[this.game_.bidingPlayerIndex];
        if(this.isPlayerAI_(bidderName)) {
            this.goAIPlayer();
        } else {
            this.dispatchEvent('nextBid', {name:bidderName, bid:this.gameBet_, lastCallerPassed: lastCallerPassed });
        }
    };

    /**
     * For the current player play his hand
     * @param {string} callerName
     * @param {number} bid  If bid 0 means the caller pass this bid
     * */
    GamePlay.prototype.callForBanker = function(callerName, bid) {
        if(bid > 0) {
            if(this.gameBet_ < bid) {
                this.highBider_ = callerName;
                this.gameBet_ = bid;
            } else {
                BOK.error('GamePlay.callForBanker: invalid bid from ['+callerName+'].');
            }
        }

        if(bid >= 3) {
            this.gameStart_();
        } else {
            //pass to next caller
            var callerIndex = BOK.findInArray(this.sideNames_, callerName);
            var nextCallerIndex = this.game_.findOutNextPlayerIndex(callerIndex);
            if(this.sideNames_[nextCallerIndex] == this.highBider_) {
                this.gameStart_();
            } else {
                this.game_.bidingPlayerIndex = nextCallerIndex;
                this.nextBidForBanker(bid == 0);
            }
        }
    };

    /**
     * For the current player play his hand
     * @param {string} player
     * @param {Array} cards contains {string}
     * @return {boolean} return true if this play is valid.
     * */
    GamePlay.prototype.playHand = function(player, cards) {
        if(this.game_.isFinished())
            return false;

        /** @type {string}*/
        var currentPlayer = this.game_.getCurrentPlayer();
        /** @type {Hand}*/
        var currentPlayerHand = this.game_.getCurrentPlayerHand();
        var playingHand = new Hand(cards, currentPlayerHand.getOwnerName(), currentPlayer);

        if(player != currentPlayer)
            return false;

        //check if current player have this hand
        if(!currentPlayerHand.checkCardsInHand(cards))
            return false;

        //check if type of playing hand is valid
        if(CoreRule.HAND_TYPE.UNKNOWN == playingHand.getType())
            return false;

        //check if hand is valid against center hand
        if(!this.game_.centerHand.isEmpty() && this.game_.centerHand.getOwner() != currentPlayer) {
            if(CoreRule.HAND_TYPE.BOOOOM == playingHand.getType()) {
                if(CoreRule.HAND_TYPE.BOOOOM == this.game_.centerHand.getType() &&
                    this.game_.centerHand.getPower() >= playingHand.getPower())
                    return false;
            } else {
                if(this.game_.centerHand.getType() != playingHand.getType() ||
                    this.game_.centerHand.getPower() >= playingHand.getPower() ) {
                    return false;
                }
            }
        }

        //if reached here meaning played hand is valid and game should proceed.
        currentPlayerHand.removeCard(cards);
        this.game_.centerHand = playingHand;

        this.dispatchEvent('play', {name:player, cards:cards});
        console.log(player + ' plays: '+cards);

        //winning check
        if(this.game_.isFinished()) {
            this.gameState_ = 'finished';
            this.dispatchEvent('gameFinished', {bankerWon:'banker' == currentPlayer});
        }
        else
            this.nextPlayer();

        return true;
    };

    /**
     * For the requesting player pass his round, if no name provided, request the
     * current player pass his round.
     * @param {string|null} [playerName]
     * */
    GamePlay.prototype.tryToPass = function(playerName) {
        if(!playerName || this.game_.getCurrentPlayerHand().getOwnerName() == playerName) {
            if(!this.game_.centerHand.isEmpty()){
                this.dispatchEvent('pass', {name:this.game_.getCurrentPlayer()});
                console.log(this.game_.getCurrentPlayer() + ' passed. ');
            }

            if(!this.game_.centerHand.isEmpty()/*able to pass*/) {
                if(this.game_.getNextPlayer() == this.game_.centerHand.getOwner()) {
                    this.game_.centerHand = new Hand();
                }
                this.nextPlayer();
            } else {
                this.AI_.go(this);
            }
        }
    };

    /**
     * For the current player pass his round
     * */
    GamePlay.prototype.nextPlayer = function() {
        this.game_.nextPlayer();
        this.callForNextPlayer_();
        this.goAIPlayer();
    };

    GamePlay.prototype.goAIPlayer = function() {
        switch(this.gameState_) {
            case 'bidding':
                var bidderName = this.sideNames_[this.game_.bidingPlayerIndex];
                if(this.isPlayerAI_(bidderName))
                    this.AI_.goBid(this, bidderName);
                break;
            case 'playing':
                if(this.isPlayerAI_(this.game_.getCurrentPlayerName()))
                    this.AI_.go(this);
                break;

            default:
                break;
        }
    };

    /**
     * For the current player pass his round
     * */
    GamePlay.prototype.gameStart_ = function() {
        this.gameState_ = 'playing';
        this.game_.setBanker(this.highBider_);

        var bankerIndex = BOK.findInArray(this.sideNames_, this.highBider_);
        var player1Index = this.game_.findOutNextPlayerIndex(bankerIndex);
        var player1Name = this.sideNames_[player1Index];
        var player2Name = this.sideNames_[this.game_.findOutNextPlayerIndex(player1Index)];
        this.dispatchEvent('gameStart',
            {
                bankerName: this.highBider_,
                player1Name: player1Name,
                player2Name: player2Name,
                bottomCards:this.game_.bottomCards
            });
        this.callForNextPlayer_();
        //go AI straight away in case the banker is AI player
        this.goAIPlayer();
    };

    GamePlay.prototype.callForNextPlayer_ = function() {
        this.dispatchEvent('nextPlayer',
            {
                name:this.game_.getCurrentPlayer(),
                allowPass:!this.game_.centerHand.isEmpty(),
                suggestCards:this.AI_.getSuggestedHand(this.game_)
            }
        );
    };

    GamePlay.prototype.enableAIForPlayer = function(playerName) {
        console.log("Now AI taking over player: "+playerName);
        var index = BOK.findInArray(this.sideNames_, playerName);
        this.enabledAI_[index] = true;
    };

    GamePlay.prototype.disableAIForPlayer = function(playerName) {
        var index = BOK.findInArray(this.sideNames_, playerName);
        this.enabledAI_[index] = false;
    };

    GamePlay.prototype.isPlayerAI_ = function(playerName) {
        var index = BOK.findInArray(this.sideNames_, playerName);
        return this.enabledAI_[index];
    };

})();
