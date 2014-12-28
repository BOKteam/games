/**
 * Created by Enveesoft.
 * User: Liu Xinyi
 * Date: 14-7-9
 * Time: 下午7:27
 * Write the description in this section.
 */

goog.provide("doudizhu.features.maingame.v.components.Table");

goog.require("doudizhu.features.maingame.v.components.Hand");
goog.require("doudizhu.features.maingame.v.components.Deck");

doudizhu.features.maingame.v.components.Table = Table;

BOK.inherits(Table, createjs.Container);
function Table() {
    createjs.Container.call(this);

    /** @type {Deck}*/
    this.deck_ = new doudizhu.features.maingame.v.components.Deck();
    this.deck_.x = 350;
    this.deck_.y = 270;
    this.addChild(this.deck_);
    //do deck init after adding it to table. otherwise new cards wont find table
    this.deck_.init();

    /** @type {Hand}*/
    this.centerHand_ = new doudizhu.features.maingame.v.components.Hand(0);
    this.centerHandPos = {
        seat0:{x:355, y:160},
        seat1:{x:510, y:160},
        seat2:{x:190, y:160}
    };
    this.hands_ = [];
    for(var i=0; i < Table.CONST.HANDS; ++i) {
        var x, y, name;
        switch(i) {
            case Table.CONST.CENTER_PLAYER_INDEX:
                name = 'banker';
                x = 50;
                y = 450;
                break;

            case Table.CONST.RIGHT_PLAYER_INDEX:
                name = 'player1';
                x = 650;
                y = 50;
                break;

            case Table.CONST.LEFT_PLAYER_INDEX:
                name = 'player2';
                x = 80;
                y = 50;
                break;
        }
        this.hands_[i] = new doudizhu.features.maingame.v.components.Hand(i, name);
        this.hands_[i].set({x:x, y:y});
        this.hands_[i].addEventListener('selectedChange', Delegate.create(this, this.onSelectedCardsChanged_));
        this.addChild(this.hands_[i]);
    }

    //
    this.bottomCardsDisplay_ = new createjs.Container();
    this.bottomCardsDisplay_.set({x:300, y:10});
    this.addChild(this.bottomCardsDisplay_);
}

Table.CONST = {
    HANDS: 3,
    CENTER_PLAYER_INDEX: 0,
    RIGHT_PLAYER_INDEX: 1,
    LEFT_PLAYER_INDEX: 2
};

/**
 * @param {Array} deck contains {string}
 * */
Table.prototype.setupTable = function(deck, bankerCard) {
    this.deck_.setup(deck, bankerCard);
    this.bottomCardsDisplay_.removeAllChildren();
};

/**
 * @param {string} playerSide
 * @param {Array} bottomCards contains {string}
 * */
Table.prototype.startGame = function(playerSide, bottomCards) {
    this.setPlayerSide(playerSide);

    var bankerHandIndex = 0;
    BOK.each(this.hands_, function(hand, i){
        if(hand.name == 'banker')
            bankerHandIndex = i;
    });

    //deal bottom cards to banker
    while(this.deck_.cardsInDeck()) {
        this.dealCardToHand_(bankerHandIndex);
    }

    //show bottom cards copy on top
    BOK.each(bottomCards, function(cardType, index){
        var card = new doudizhu.features.maingame.v.components.Card(cardType);
        card.show();
        card.set({x: (index - 1) * 50, scaleX: 0.7, scaleY: 0.7});
        this.bottomCardsDisplay_.addChild(card);
    }, this);

    //enable player hand after all set
    setTimeout(Delegate.create(this, this.enablePlayerHand), 700);
};

/**
 * @param {string} side
 * */
Table.prototype.setPlayerSide = function(side) {
    var sideName = side;
    BOK.each(this.hands_, function(hand) {
        hand.name = sideName;
        sideName = new CoreRule().getNextPlayer(sideName);
    }, this);
};

/**
 * @param {string} name
 * @param {Array} cards contains {string}
 * */
Table.prototype.playerPlayCards = function(name, cards) {
    var cardsPlayed = this.getHandByName(name).playCards(cards);
    var seat = this.getHandIndexByName(name);
    switch(this.getHandIndexByName(name)){
        case Table.CONST.LEFT_PLAYER_INDEX:
            this.centerHand_.set(this.centerHandPos['seat' + seat]);
            break;
        case Table.CONST.RIGHT_PLAYER_INDEX:
            this.centerHand_.set({x: this.centerHandPos['seat' + seat].x - (cards.length - 1)  * 35, y: this.centerHandPos['seat' + seat].y});
            break;
        case Table.CONST.CENTER_PLAYER_INDEX:
            this.centerHand_.set({x: this.centerHandPos['seat' + seat].x - (cards.length - 1) / 2 * 35, y: this.centerHandPos['seat' + seat].y});
            break;
    }

    this.deck_.addCards(this.centerHand_.empty());
    BOK.each(cardsPlayed, function(card){
        this.centerHand_.addCard(card);
    }, this);
};

Table.prototype.getHandByName = function(name) {
    var foundHand = null;
    BOK.each(this.hands_, function(hand){
        if(hand.name == name) {
            foundHand = hand;
            return BOK.BREAK;
        }
    });

    return foundHand;
};

Table.prototype.getHandIndexByName = function(name) {
    return BOK.findInArray(this.hands_, this.getHandByName(name));
};

/**
 * @param {number} startSide
 * */
Table.prototype.dealNextCard = function(startSide) {
    //reserve last 3 cards
    if(this.deck_.cardsInDeck() > 3) {
        this.dealCardToHand_(((this.deck_.cardsInDeck() + startSide) * 2) % 3);
        return true;
    }

    return false;
};


Table.prototype.enablePlayerHand = function() {
    this.hands_[0].enable();
};

Table.prototype.resetSelected = function(name) {
    if(this.hands_[0].name == name)
        this.hands_[0].resetSelectedCards();
};

Table.prototype.dealCardToHand_ = function(handNumber) {
    if(handNumber < 0 || handNumber >= Table.CONST.HANDS) {
        BOK.error('Table.dealCardToHand invalid hand number ' + handNumber);
        return;
    }

    var card = this.deck_.getTopCard();
    card && this.hands_[handNumber].addCard(card);
};

Table.prototype.onSelectedCardsChanged_ = function(e) {
    var hand = e.target;

    if(hand.isUserPlayer()) {
        var event = new createjs.Event('selectedChange');
        event.body = {name: hand.name, cards:hand.getSelectedCards()};
        this.dispatchEvent(event);
    }
};




