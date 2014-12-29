/**
 * Created by lys.
 * User: Liu Xinyi
 * Date: 14-7-9
 * Time: 下午1:13
 * Write the description in this section.
 */
goog.provide("doudizhu.features.maingame.v.TableMediator");
goog.require("bok.framework.core.BaseMediator");

goog.require("doudizhu.features.maingame.v.components.Table");

doudizhu.features.maingame.v.TableMediator = TableMediator;

BOK.inherits(TableMediator, BaseMediator);
function TableMediator(stage) {
    BaseMediator.call(this);

    this.table_ = null;
    this.stage_ = new createjs.Container();
    this.stage_.addChild(new createjs.Bitmap('assets/img/GameUI/bg.png'));

    stage.addChild(this.stage_);
}

/**
 * @override
 * */
TableMediator.prototype.declareInterestedNotifications = function() {
    this.declareInterest(CoreFeatureNotes.getOutputNoteWithFeatureName('HAND_PLAYED'), this.onPlayHand_, BaseMediator.SCOPE.PARENT);
    this.declareInterest(CoreFeatureNotes.getOutputNoteWithFeatureName('PLAYER_PASSED'), this.onPassed_, BaseMediator.SCOPE.PARENT);
    this.declareInterest(CoreFeatureNotes.getOutputNoteWithFeatureName('GAME_INIT'), this.onGameInit_, BaseMediator.SCOPE.PARENT);
    this.declareInterest(CoreFeatureNotes.getOutputNoteWithFeatureName('GAME_START'), this.onGameStart_, BaseMediator.SCOPE.PARENT);
    this.declareInterest(GameUIFeatureNotes.getOutputNoteWithFeatureName('BACK_TO_MENU'), this.onQuitGame_, BaseMediator.SCOPE.PARENT);
    this.declareInterest(GameUIFeatureNotes.getOutputNoteWithFeatureName('PLAY_HINT'), this.onPayHint_, BaseMediator.SCOPE.PARENT);

    TableMediator.superClass_.declareInterestedNotifications.call(this);
};


/***************** Notification Handlers ***********************/
TableMediator.prototype.onGameInit_ = function(msg) {
    //re-init table
    if(this.table_)
        this.stage_.removeChild(this.table_);
    this.table_ = new doudizhu.features.maingame.v.components.Table();
    this.table_.addEventListener('selectedChange', Delegate.create(this, this.onPlayerSelectedCardsChanged_));
    this.stage_.addChild(this.table_);

    //setup table with data
    this.playerName_ = msg.body.playerName;
    var playerSide = BOK.findInArray(msg.body.sideNames, this.playerName_);
    var deck = msg.body.deck;
    var bankerCard = msg.body.bankerCard;
    this.table_.setupTable(deck, bankerCard);

    //start dealing cards
    var THIS = this;
    function dealCards() {
        setTimeout(function(){
            if(THIS.table_){
                if(THIS.table_.dealNextCard(playerSide))
                    dealCards();
                else
                    setTimeout(function(){
                        THIS.sendParentNotification(CoreFeatureNotes.getInputNoteWithFeatureName('BID_READY_TO_START'));
                    }, 1000);
            }
        }, 100);
    }
    dealCards();
};

TableMediator.prototype.onGameStart_ = function(msg) {
    var info = msg.body;
    if(info.bankerName == this.playerName_) {
        this.playerSide_ = 'banker';
    } else if(info.player1Name == this.playerName_){
        this.playerSide_ = 'player1';
    } else if(info.player2Name == this.playerName_){
        this.playerSide_ = 'player2';
    }

    this.table_.startGame(this.playerSide_, msg.body.bottomCards);
};

TableMediator.prototype.onPlayHand_ = function(msg) {
    var name = msg.body.name;
    var cards = msg.body.cards;
    this.table_.playerPlayCards(name, cards);
};

TableMediator.prototype.onPassed_ = function(msg) {
    if(msg.body.name == this.playerSide_) {
        this.table_.resetSelected(msg.body.name);
    }
};

TableMediator.prototype.onQuitGame_ = function() {
    //reset table on quit
    if(this.table_){
        this.stage_.removeChild(this.table_);
        this.table_ = null;
    }
};

TableMediator.prototype.onPayHint_ = function(msg) {
    //hint game play
    if(msg.body.name == this.playerSide_) {
        var hand = this.table_.getHandByName(msg.body.name);
        hand.selectCards(msg.body.cards);
    }
};

/***************** Event Listener ***********************/
TableMediator.prototype.onPlayerSelectedCardsChanged_ = function(e) {
    this.sendNotification(MainGameFeatureNotes.getOutputNote('HAND_SELECTED'), BOK.cloneObject(e.body));
};

