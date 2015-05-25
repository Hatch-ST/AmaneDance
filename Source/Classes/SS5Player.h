// 
//  SS5Player.h
//


/************************************************************

- Quick start
 
  #include "SS5Player.h"
 
  ss::ResourceManager* resman = ss::ResourceManager::getInstance();
  resman->addData("sample.ssbp");

  ss::Player* player = ss::Player::create();
  player->setData("sample");			// ssbp�t�@�C�����i�g���q�s�v�j
  player->play("anime1");				//
  CCPoint pos(200,200);
  ssplayer->setPosition(pos);
  this->addChild(player);

  SS5Player cocos2d-x ver2.x�ł͏I�������� resman �� delete ���Ă��������B

  ver2.X�n��SS5Player��X��]�AY��]�ɔ�Ή��ł��B

*************************************************************/


#ifndef SS5Player_h
#define SS5Player_h

#include "cocos2d.h"
#include "SS5PlayerData.h"


namespace ss
{
class SSPlayerDelegate;
class CustomSprite;
class CellCache;
class CellRef;
class AnimeCache;
class AnimeRef;
class ResourceSet;
struct ProjectData;

/**
 * ResourceManager
 */
//class ResourceManager : public cocos2d::Ref
class ResourceManager
{
public:
	static const std::string s_null;

	/**
	 * �f�t�H���g�C���X�^���X���擾���܂�.
	 *
	 * @return �f�t�H���g��ResourceManager�C���X�^���X
	 */
	static ResourceManager* getInstance();

	/**
	 * ssbp�t�@�C����ǂݍ��݊Ǘ��ΏۂƂ��܂�.
	 * dataKey��ssbp�̃t�@�C�����i�g���q�Ȃ��j�ɂȂ�܂�.
	 *
	 * @param  ssbpFilepath  ssbp�t�@�C���̃p�X
	 * @param  imageBaseDir  �摜�t�@�C���̓ǂݍ��݌����[�g�p�X. �ȗ�����ssbp�̂���ꏊ�����[�g�Ƃ��܂�.
	 * @return dataKey
	 */
	std::string addData(const std::string& ssbpFilepath, const std::string& imageBaseDir = s_null);

	/**
	 * ssbp�t�@�C����ǂݍ��݊Ǘ��ΏۂƂ��܂�.
	 *
	 * @param  dataKey       dataKey�̎w��
	 * @param  ssbpFilepath  ssbp�t�@�C���̃p�X
	 * @param  imageBaseDir  �摜�t�@�C���̓ǂݍ��݌����[�g�p�X. �ȗ�����ssbp�̂���ꏊ�����[�g�Ƃ��܂�.
	 * @return dataKey
	 */
	std::string addDataWithKey(const std::string& dataKey, const std::string& ssbpFilepath, const std::string& imageBaseDir = s_null);

	/**
	 * �w�肳�ꂽssbp�f�[�^���Ǘ��ΏۂƂ��܂�.
	 *
	 * @param  dataKey       dataKey�̎w��
	 * @param  data          ssbp�f�[�^
	 * @param  imageBaseDir  �摜�t�@�C���̓ǂݍ��݌����[�g�p�X. �ȗ�����ssbp�̂���ꏊ�����[�g�Ƃ��܂�.
	 * @return dataKey
	 */
	std::string addData(const std::string& dataKey, const ProjectData* data, const std::string& imageBaseDir = s_null);
	
	/**
	 * �w��f�[�^��������܂�.
	 *
	 * @param  dataKey
	 */
	void removeData(const std::string& dataKey);

	/**
	 * �S�Ẵf�[�^��������܂�.
	 */
	void removeAllData();

	/**
	* ���O�ɑΉ�����f�[�^�擾���܂�.
	*/
	ResourceSet* getData(const std::string& dataKey);

	/**
	* �w�肵���Z���̃e�N�X�`����ύX���܂�.
	* @param  dataName       ssbp���i�g���q�������t�@�C�����j
	* @param  callName       ssce���i�g���q�������t�@�C�����j
	* @param  texture        �ύX��̃e�N�X�`��
	*                        �e�N�X�`���̓|�C���^���Q�Ƃ��邾���Ȃ̂ŁA�g�p���鑤�Ŏ��̂��Ǘ�����K�v������܂��B
	* @return �ύX���s������
	*/
	bool changeTexture(char* dataName, char* callName, cocos2d::CCTexture2D* texture);

	/**
	 * �V����ResourceManager�C���X�^���X���\�z���܂�.
	 *
	 * @return ResourceManager�C���X�^���X
	 */
	static ResourceManager* create();

public:
	ResourceManager(void);
	virtual ~ResourceManager();

protected:
//	cocos2d::Map<std::string, ResourceSet*>	_dataDic;
	std::map<std::string, ResourceSet*>	_dataDic;
};



/**
 * UserData
 */
struct UserData
{
	enum {
		FLAG_INTEGER	= 1 << 0,
		FLAG_RECT		= 1 << 1,
		FLAG_POINT		= 1 << 2,
		FLAG_STRING		= 1 << 3
	};

	const char*	partName;		// Part name
	int			frameNo;		// Frame no

	int			flags;			// Flags of valid data
	int			integer;		// Integer
	int			rect[4];		// Rectangle Left, Top, Right, Bottom
	int			point[2];		// Position X, Y
	const char*	str;			// String (zero terminated)
	int			strSize;		// String size (byte count)
};


/**
* LabelData
*/
struct LabelData
{
	std::string	str;			// String (zero terminated)
	int			strSize;		// String size (byte count)
	int			frameNo;		// Frame no
};

/**
* ResluteState
*/
struct ResluteState
{
	float	x;
	float	y;
};

//�܂܂��p�[�c�f�[�^�t���O
enum {
	PART_FLAG_INVISIBLE = 1 << 0,
	PART_FLAG_FLIP_H = 1 << 2,
	PART_FLAG_FLIP_V = 1 << 3,

	// optional parameter flags
	PART_FLAG_CELL_INDEX = 1 << 4,
	PART_FLAG_POSITION_X = 1 << 5,
	PART_FLAG_POSITION_Y = 1 << 6,
	PART_FLAG_ANCHOR_X = 1 << 7,
	PART_FLAG_ANCHOR_Y = 1 << 8,
	PART_FLAG_ROTATIONX = 1 << 9,
	PART_FLAG_ROTATIONY = 1 << 10,
	PART_FLAG_ROTATIONZ = 1 << 11,
	PART_FLAG_SCALE_X = 1 << 12,
	PART_FLAG_SCALE_Y = 1 << 13,
	PART_FLAG_OPACITY = 1 << 14,
	PART_FLAG_COLOR_BLEND = 1 << 15,
	PART_FLAG_VERTEX_TRANSFORM = 1 << 16,

	PART_FLAG_SIZE_X = 1 << 17,
	PART_FLAG_SIZE_Y = 1 << 18,

	PART_FLAG_U_MOVE = 1 << 19,
	PART_FLAG_V_MOVE = 1 << 20,
	PART_FLAG_UV_ROTATION = 1 << 21,
	PART_FLAG_U_SCALE = 1 << 22,
	PART_FLAG_V_SCALE = 1 << 23,
	PART_FLAG_BOUNDINGRADIUS = 1 << 24,

	PART_FLAG_INSTANCE_KEYFRAME = 1 << 25,
	PART_FLAG_INSTANCE_START = 1 << 26,
	PART_FLAG_INSTANCE_END = 1 << 27,
	PART_FLAG_INSTANCE_SPEED = 1 << 28,
	PART_FLAG_INSTANCE_LOOP = 1 << 29,
	PART_FLAG_INSTANCE_LOOP_FLG = 1 << 30,

	NUM_PART_FLAGS
};

//���_�ό`�t���O
enum {
	VERTEX_FLAG_LT = 1 << 0,
	VERTEX_FLAG_RT = 1 << 1,
	VERTEX_FLAG_LB = 1 << 2,
	VERTEX_FLAG_RB = 1 << 3,
	VERTEX_FLAG_ONE = 1 << 4	// color blend only
};

//�C���X�^���X�t���O
enum {
	INSTANCE_LOOP_FLAG_INFINITY = 1 << 0,
	INSTANCE_LOOP_FLAG_REVERSE = 1 << 1,
	INSTANCE_LOOP_FLAG_PINGPONG = 1 << 2,
	INSTANCE_LOOP_FLAG_INDEPENDENT = 1 << 3,
};

/// Animation Part Type
enum
{
	PARTTYPE_INVALID = -1,
	PARTTYPE_NULL,			// null�B�̈��������SRT���̂݁B�������~�`�̓����蔻��͐ݒ�\�B
	PARTTYPE_NORMAL,		// �ʏ�p�[�c�B�̈�����B�摜�͖����Ă������B
	PARTTYPE_TEXT,			// �e�L�X�g(�\��@�������j
	PARTTYPE_INSTANCE,		// �C���X�^���X�B���A�j���A�p�[�c�ւ̎Q�ƁB�V�[���ҏW���[�h�̑�ւɂȂ����
	PARTTYPE_NUM
};

//�����蔻��̎��
enum
{
	INVALID = -1,
	NONE,			///< �����蔻��Ƃ��Ďg��Ȃ��B
	QUAD,			///< ���݂ɕό`����l�ӌ`�B���_�ό`�ȂǓK�p��̂S�p�����񂾗̈�B�ł��d���B
	AABB,			///< ��]���Ȃ��S�̂��͂ދ�`�Ō�������
	CIRCLE,			///< �^�~�̔��a�ŋ����ɂ�蔻�肷��
	CIRCLE_SMIN,	///< �^�~�̔��a�ŋ����ɂ�蔻�肷�� (�X�P�[����x,y�̍ŏ��l���Ƃ�j
	CIRCLE_SMAX,	///< �^�~�̔��a�ŋ����ɂ�蔻�肷�� (�X�P�[����x,y�̍ő�l���Ƃ�j
	num
};

//���u�����h���@
enum BlendType
{
	BLEND_MIX,		///< 0 �u�����h�i�~�b�N�X�j
	BLEND_MUL,		///< 1 ��Z
	BLEND_ADD,		///< 2 ���Z
	BLEND_SUB		///< 3 ���Z
};

//�Œ菭���̒萔 10=1�h�b�g
#define DOT (10.0f)
#define PART_VISIBLE_MAX (512)
/**
 * Player
 */
class Player : public cocos2d::CCSprite
{
public:
	typedef void (cocos2d::CCObject::*SEL_PlayEndHandler)(Player*);

	/**
	 * Player�C���X�^���X���\�z���܂�.
	 *
	 * @param  resman  �g�p����ResourceManager�C���X�^���X. �ȗ����̓f�t�H���g�C���X�^���X���g�p����܂�.
	 * @return Player�C���X�^���X
	 */
	static Player* create(ResourceManager* resman = NULL);

	/**
	 * �g�p����ResourceManager�C���X�^���X��ݒ肵�܂�.
	 *
	 * @param  resman  �g�p����ResourceManager�C���X�^���X. �ȗ����̓f�t�H���g�C���X�^���X���g�p����܂�.
	 */
	void setResourceManager(ResourceManager* resman = NULL);

	/**
	 * �g�p����ResourceManager�C���X�^���X��������܂�.
	 * �ēxResourceManager�C���X�^���X��ݒ肷��܂ł͍Đ��ł��Ȃ��Ȃ�܂�.
	 */
	void releaseResourceManager();

	/**
	 * �Đ�����ssbp�f�[�^��dataKey��ݒ肵�܂�.
	 *
	 * @param  dataKey  �Đ�����f�[�^��dataKey
	 */
	void setData(const std::string& dataKey);

	/**
	 * �ݒ肳��Ă���ssbp�f�[�^��������܂�.
	 */
	void releaseData();

	/**
	 * �ݒ肳��Ă���A�j���[�V������������܂�.
	 */
	void releaseAnime();

	/**
	 * �A�j���[�V�����̍Đ����J�n���܂�.
	 *
	 * @param  packName      �p�b�N��(ssae�j
	 * @param  animeName     �Đ�����A�j���[�V������
	 * @param  loop          �Đ����[�v���̎w��. �ȗ�����0
	 * @param  startFrameNo  �Đ����J�n����t���[��No�̎w��. �ȗ�����0
	 */
	void play(const std::string& packName, const std::string& animeName, int loop = 0, int startFrameNo = 0);

	/**
	 * �A�j���[�V�����̍Đ����J�n���܂�.
	 * �A�j���[�V����������Đ�����f�[�^��I�����܂�.
	 * "ssae��/�A�j���[�V������" �Ƃ����w�肪�\�ł�.
	 * sample.ssae��anime_1���w�肷��ꍇ�Asample/anime_1�ƂȂ�܂�.
	 * ���A�j���[�V�������݂̂Ŏw�肵���ꍇ�A�����̃A�j���[�V�������������ݎ��ɂǂ̃A�j���[�V�������I������邩�͕s��ł�.
	 *
	 * @param  animeName     �Đ�����A�j���[�V������
	 * @param  loop          �Đ����[�v���̎w��. �ȗ�����0
	 * @param  startFrameNo  �Đ����J�n����t���[��No�̎w��. �ȗ�����0
	 */
	void play(const std::string& animeName, int loop = 0, int startFrameNo = 0);

	/**
	 * �Đ��𒆒f���܂�.
	 */
	void pause();

	/**
	 * �Đ����ĊJ���܂�.
	 */
	void resume();

	/**
	 * �Đ����~���܂�.
	 * �Q�[�����ŃA�j���[�V�����̕\���t���[���𐧌䂷��ꍇ��stop()���Ăяo������
	 * �Q�[�����̍X�V������setFrameNo()���Ăяo���w��̃t���[����\�����Ă��������B
	 */
	void stop();

	/**
	 * �Đ����Ă���A�j���[�V�����̃p�b�N��(ssae)��Ԃ��܂�.
	 *
	 * @return �p�b�N��(ssae)
	 */
	const std::string& getPlayPackName() const;

	/**
	 * �Đ����Ă���A�j���[�V��������Ԃ��܂�.
	 *
	 * @return �A�j���[�V������
	 */
	const std::string& getPlayAnimeName() const;
	
	/**
	* �A�j���[�V�����̑��t���[�����擾���܂�.
	*
	* @return ���t���[��
	*/
	int getMaxFrame() const;

	/**
	 * �Đ��t���[��No���擾���܂�.
	 * Get frame no of playing.
	 *
	 * @return �Đ��t���[��No. frame no.
	 */
	int getFrameNo() const;

	/**
	 * �Đ��t���[��No��ݒ肵�܂�.
	 * Set frame no of playing.
	 *
	 * @param �Đ��t���[��No. frame no.
	 */
	void setFrameNo(int frameNo);

	/**
	 * �Đ��X�s�[�h���擾���܂�. (1.0f:�W��)
	 * Set speed to play. (1.0f:normal speed)
	 */
	float getStep() const;

	/**
	 * �Đ��X�s�[�h��ݒ肵�܂�. (1.0f:�W��)
	 * Get speed to play. (1.0f:normal speed)
	 */
	void setStep(float step);
	
	/** 
	 * �w�肳��Ă���Đ����[�v�񐔂��擾���܂�. (0:�w��Ȃ�)
	 * Get a playback loop count of specified. (0:not specified)
	 */
	int getLoop() const;

	/** 
	 * �Đ����[�v�񐔂�ݒ肵�܂�. (0:�w��Ȃ�)
	 * Set a playback loop count.  (0:not specified)
	 */
	void setLoop(int loop);

	/** 
	 * ���݂܂ł̃��[�v�Đ��񐔂��擾���܂�.
	 * Get repeat count a playback.
	 */
	int getLoopCount() const;

	/** 
	 * ���݂܂ł̃��[�v�Đ��񐔂��N���A���܂�.
	 * Clear repeat count a playback.
	 */
	void clearLoopCount();

	/**
	 * �t���[���X�L�b�v�i�t���[�����[�g�ɍ��킹�Đ��t���[�����X�L�b�v����j�̐ݒ�����܂�. (default: true)
	 * Set frame-skip(to skip the playback frame according to the frame rate). (default: true)
	 */
	void setFrameSkipEnabled(bool enabled);
	
	/** 
	 * �t���[���X�L�b�v�̐ݒ��Ԃ�Ԃ��܂�.
	 * Get frame-skip setting.
	 */
	bool isFrameSkipEnabled() const;

	/**
	* ���x��������t���[���ʒu���擾���܂�.
	*/
	int getLabelToFrame(char* findLabelName);

	/**
	* index����p�[�c�����擾���܂�.
	*/
	const char* getPartName(int partId) const;

	/**
	* �p�[�c������index���擾���܂�.
	*/
	int indexOfPart(const char* partName) const;

	/**
	* �p�[�c�̖�����A�p�[�c�����擾���܂�.
	*/
	bool getPartState(ResluteState& result, const char* name, int frameNo = -1);

	/**
	* �p�[�c�̕\���A��\����ݒ肵�܂�.
	*/
	void setPartVisible( int partNo, bool flg );

	/** ���[�U�[�f�[�^�Ȃǂ̒ʒm���󂯎��A�f���Q�[�g��ݒ肵�܂�.
	 *  Set delegate. receive a notification, such as user data.
	 *
	 *  @code
	 *  player->setDelegate((SSPlayerDelegate *)this);
	 *  --
	 *
	 *  void MyScene::onUserData(ss::Player* player, const ss::UserData* data)
	 *  {
	 *    ...
	 *  }
	 *  @endcode
	 */
	void setDelegate(SSPlayerDelegate* delegate);
    
	/** �Đ��I���̒ʒm���󂯂�R�[���o�b�N��ݒ肵�܂�.
	 *
     *  @code
	 *  player->setPlayEndCallback(this, ssplayer_playend_selector(MyScene::playEndCallback));
	 *  --
	 *  void MyScene::playEndCallback(ss::Player* player)
	 *  {
	 *    ...
	 *  }
     *  @endcode
	 */
	void setPlayEndCallback(cocos2d::CCObject* target, SEL_PlayEndHandler selector);
    



public:
	Player(void);
	virtual ~Player();

	// override
	virtual bool init();
	virtual void update(float dt);

protected:
	void allocParts(int numParts, bool useCustomShaderProgram);
	void releaseParts();
	void setPartsParentage();

	void play(AnimeRef* animeRef, int loop, int startFrameNo);
	void updateFrame(float dt);
	void setFrame(int frameNo);
	void checkUserData(int frameNo);
	void get_uv_rotation(float *u, float *v, float cu, float cv, float deg);
	void set_InstanceAlpha(int alpha);

protected:
	ResourceManager*	_resman;
	ResourceSet*		_currentRs;
	std::string			_currentdataKey;
	AnimeRef*			_currentAnimeRef;
	std::vector<CustomSprite *>	_parts;

	bool				_frameSkipEnabled;
	float				_playingFrame;
	float				_step;
	int					_loop;
	int					_loopCount;
	bool				_isPlaying;
	bool				_isPausing;
	int					_prevDrawFrameNo;
	bool				_partVisible[PART_VISIBLE_MAX];
	int					_InstanceAlpha;

	SSPlayerDelegate*	_delegate;
	UserData			_userData;
    CCObject*			_playEndTarget;
	SEL_PlayEndHandler	_playEndSelector;
};

#define ssplayer_playend_selector(_SELECTOR) (ss::Player::SEL_PlayEndHandler)(&_SELECTOR)

/**
 * SSPlayerDelegate
 */

class SSPlayerDelegate
{
public:
    SSPlayerDelegate(void) {}
    virtual ~SSPlayerDelegate();
    
    /** ���[�U�[�f�[�^�̎�M
     *  Receive a user data.
     */
	virtual void onUserData(ss::Player* player, const ss::UserData* data);
};


};	// namespace ss

#endif