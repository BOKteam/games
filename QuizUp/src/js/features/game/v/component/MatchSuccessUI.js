/**
 * Created by Envee.
 *
 * Date: 14-11-07
 * Time: pm 16:17
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */


goog.provide("quiz.features.game.v.component.MatchSuccessUI");
goog.require("org.createjs.easeljs.EaselJS");
goog.require("bok.util.EaselAnimationHelper");
goog.require("quiz.features.game.v.component.PKIcon");
goog.require('quiz.AssetsList');
goog.require("quiz.features.game.v.component.Avatar");

quiz.features.game.v.component.MatchSuccessUI = MatchSuccessUI;
BOK.inherits(MatchSuccessUI, createjs.Container);

function MatchSuccessUI (){
	createjs.Container.call(this);

	var space = CONST.MATCH_SUCCESS.CENTER_SPACE / 2; 
	var panelHeight = CONST.BG.HEIGHT / 2 - space;
	this.topPanel_ = new createjs.Container();
	this.topPanel_.addChild(new createjs.Shape(new createjs.Graphics().beginFill('#48366D').drawRect(0, 0, CONST.BG.WIDTH,  panelHeight).endFill()));
	this.topPanel_.set(MatchSuccessUI.settings.panelTop)

	this.downPanel_ = new createjs.Container();
	this.downPanel_.addChild(new createjs.Shape(new createjs.Graphics().beginFill('#67696B').drawRect(0, 0, CONST.BG.WIDTH, panelHeight ).endFill()));
	this.downPanel_.set(MatchSuccessUI.settings.panelDown)

	this.addChild(this.topPanel_);
	this.addChild(this.downPanel_);
	var avatarRadius = CONST.MATCH_SUCCESS.AVATAR_RADIUS;
	this.avatarT_ = new quiz.features.game.v.component.Avatar(avatarRadius, "#FFBE1A", null, 0.6 );
	this.avatarT_.set({x: CONST.BG.WIDTH * 19 / 20 - avatarRadius, y: panelHeight / 2});
	this.topPanel_.addChild(this.avatarT_);

	this.avatarD_ = new quiz.features.game.v.component.Avatar(avatarRadius, "#FF5454", null, 0.6 );
	this.avatarD_.set({x: CONST.BG.WIDTH * 1 / 20 + avatarRadius, y: panelHeight / 2});
	this.downPanel_.addChild(this.avatarD_);

	this.centerPKIcon_ = new createjs.Container();
	this.centerPKIcon_.set(MatchSuccessUI.settings.pkIcon);
	this.centerPKIcon_.visible = false;
	this.playerInfo_ = [
        new createjs.Container(),
        new createjs.Container()
    ];
    this.playerInfo_[0].set(MatchSuccessUI.settings.playerT);
    this.playerInfo_[1].set(MatchSuccessUI.settings.playerD);
 	this.downPanel_.addChild(this.playerInfo_[1]);
 	this.topPanel_.addChild(this.playerInfo_[0]);
     //init profile
    this.profileT_ = {
        nameText : new createjs.Text("John","45px " + CONST.FONT.ALL, "#FFFFFF"),
        countryText : new createjs.Text("Washiton","20px " + CONST.FONT.ALL, "#FFFFFF")
    };
    var i = 0;
    BOK.each(this.profileT_, function(profile){
        profile.set({x: 0, y: 55 * i  , textAlign: "right"});
        this.playerInfo_[0].addChild(profile);
        i ++;
    }, this);


    this.profileD_ = {
        nameText : new createjs.Text("John","45px " + CONST.FONT.ALL, "#FFFFFF"),
        countryText : new createjs.Text("Washiton","20px " + CONST.FONT.ALL, "#FFFFFF")
    };

    i =0;
    BOK.each(this.profileD_, function(profile){
        profile.set({x: 0, y: 55 * i, textAlign: "left"});
        this.playerInfo_[1].addChild(profile);
        i ++;
    }, this);

	this.initCenter_();
	this.addChild(this.centerPKIcon_);
	this.visible = false;
}
MatchSuccessUI.settings = {
	playerT: {x: CONST.BG.WIDTH * 19 / 20 - CONST.MATCH_SUCCESS.AVATAR_RADIUS * 2- CONST.MATCH_SUCCESS.PLAYERINFO_WIDTH + CONST.MATCH_SUCCESS.PLAYERINFO_WIDTH * 3/ 4, y: (CONST.BG.HEIGHT / 2 - CONST.MATCH_SUCCESS.CENTER_SPACE) / 2 - CONST.MATCH_SUCCESS.AVATAR_RADIUS * 3 / 4},
	playerD:{x: CONST.BG.WIDTH *  1 / 20 + CONST.MATCH_SUCCESS.AVATAR_RADIUS * 2 + CONST.MATCH_SUCCESS.PLAYERINFO_WIDTH - CONST.MATCH_SUCCESS.PLAYERINFO_WIDTH * 3/ 4, y: (CONST.BG.HEIGHT / 2 - CONST.MATCH_SUCCESS.CENTER_SPACE) / 2 - CONST.MATCH_SUCCESS.AVATAR_RADIUS * 3/ 4},
	panelTop: {x: 0, y: 0},
	panelDown: {x:0, y:CONST.BG.HEIGHT / 2 +  CONST.MATCH_SUCCESS.CENTER_SPACE / 2},
	pkIcon: {x: 0, y: CONST.BG.HEIGHT / 2 -  CONST.MATCH_SUCCESS.CENTER_SPACE / 2}
};

MatchSuccessUI.prototype.init = function(playerInfos){
		this.setPlayerInfos_(playerInfos);
};
MatchSuccessUI.prototype.show = function(){
	var panelHeight = CONST.BG.HEIGHT / 2 - CONST.MATCH_SUCCESS.CENTER_SPACE / 2;
	this.visible = true;
	this.topPanel_.y = this.topPanel_.y - panelHeight;
	this.downPanel_.y = this.downPanel_.y + panelHeight;
	EaselAnimationHelper.moveTo(this.topPanel_,MatchSuccessUI.settings.panelTop.x, MatchSuccessUI.settings.panelTop.y, 1500);
	createjs.Tween.get(this.downPanel_).to(MatchSuccessUI.settings.panelDown, 1000).call(Delegate.create(this, function(){
		this.centerPKIcon_.x = MatchSuccessUI.settings.pkIcon.x + CONST.BG.WIDTH;
		this.centerPKIcon_.visible = true;
		EaselAnimationHelper.moveTo(this.centerPKIcon_,MatchSuccessUI.settings.pkIcon.x, MatchSuccessUI.settings.pkIcon.y, 500);
	}));
};

MatchSuccessUI.prototype.hide = function(){
	var panelHeight = CONST.BG.HEIGHT / 2 - CONST.MATCH_SUCCESS.CENTER_SPACE / 2;
	EaselAnimationHelper.moveTo(this.topPanel_,MatchSuccessUI.settings.panelTop.x, MatchSuccessUI.settings.panelTop.y - panelHeight, 1000);

	EaselAnimationHelper.moveTo(this.downPanel_, this.downPanel_.x,  MatchSuccessUI.settings.panelDown.y + panelHeight, 1000);
	createjs.Tween.get(this.centerPKIcon_).to({x:MatchSuccessUI.settings.pkIcon.x - CONST.BG.WIDTH, y:MatchSuccessUI.settings.pkIcon.y}, 500).call(Delegate.create(this, function(){
				this.visible = false;
				this.centerPKIcon_.visible = false;
		}));


};

MatchSuccessUI.prototype.initCenter_ = function(){
	var line = new createjs.Shape(new createjs.Graphics().beginFill('#FFBE1A').drawRect(0, 0, CONST.BG.WIDTH, CONST.MATCH_SUCCESS.CENTER_SPACE).drawCircle(CONST.BG.WIDTH / 2, CONST.MATCH_SUCCESS.CENTER_SPACE / 2, CONST.MATCH_SUCCESS.CENTER_ROUND_RADIUS));
	var pkIcon =  new quiz.features.game.v.component.PKIcon(CONST.MATCH_SUCCESS.CENTER_PK_WIDTH).set({x:CONST.BG.WIDTH / 2 - CONST.MATCH_SUCCESS.CENTER_PK_WIDTH / 2, y: CONST.MATCH_SUCCESS.CENTER_SPACE / 2  - CONST.MATCH_SUCCESS.CENTER_PK_WIDTH / 2});
	this.centerPKIcon_.addChild(line, pkIcon);
};

MatchSuccessUI.prototype.setPlayerInfos_  = function(playerInfos){
	var player1 = playerInfos[0];
    var player2 = playerInfos[1];
    this.setProfile_(player1, this.profileT_);
    this.setProfile_(player2, this.profileD_);
    //set avatars
    this.avatarT_.setAvatar(player1.avatar);
    this.avatarD_.setAvatar(player2.avatar);
} ;


MatchSuccessUI.prototype.setProfile_ = function(player, profile){
    profile.nameText.text = player.name;
    profile.countryText.text = player.seat;
};