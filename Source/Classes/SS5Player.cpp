// 
//  SS5Player.cpp
//
#include "SS5Player.h"
#include "SS5PlayerData.h"
#include <string>


namespace ss
{

/**
 * definition
 */

static const ss_u32 DATA_ID = 0x42505353;
static const ss_u32 DATA_VERSION = 1;


/**
 * utilites
 */

static void splitPath(std::string& directoty, std::string& filename, const std::string& path)
{
    std::string f = path;
    std::string d = "";

    size_t pos = path.find_last_of("/");
	if (pos == std::string::npos) pos = path.find_last_of("\\");	// for win

    if (pos != std::string::npos)
    {
        d = path.substr(0, pos+1);
        f = path.substr(pos+1);
    }

	directoty = d;
	filename = f;
}

// printf �`���̃t�H�[�}�b�g
static std::string Format(const char* format, ...){

	static std::vector<char> tmp(1000);

	va_list args, source;
	va_start(args, format);
	source = args;

	while (1)
	{
		args = source;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        //iOS
		if (vsnprintf(&tmp[0], tmp.size(), format, args) == -1)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        //Android
        if (vsnprintf(&tmp[0], tmp.size(), format, args) == -1)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		//Windows
		if (_vsnprintf(&tmp[0], tmp.size(), format, args) == -1)
#endif
		{
			tmp.resize(tmp.size() * 2);
		}
		else
		{
			break;
		}
	}
	tmp.push_back('\0');
	std::string ret = &(tmp[0]);
	va_end(args);
	return ret;
}





/**
 * ToPointer
 */
class ToPointer
{
public:
	explicit ToPointer(const void* base)
		: _base(static_cast<const char*>(base)) {}
	
	const void* operator()(ss_offset offset) const
	{
		return (_base + offset);
	}

private:
	const char*	_base;
};


/**
 * DataArrayReader
 */
class DataArrayReader
{
public:
	DataArrayReader(const ss_u16* dataPtr)
		: _dataPtr(dataPtr)
	{}

	ss_u16 readU16() { return *_dataPtr++; }
	ss_s16 readS16() { return static_cast<ss_s16>(*_dataPtr++); }

	unsigned int readU32()
	{
		unsigned int l = readU16();
		unsigned int u = readU16();
		return static_cast<unsigned int>((u << 16) | l);
	}

	int readS32()
	{
		return static_cast<int>(readU32());
	}

	float readFloat()
	{
		union {
			float			f;
			unsigned int	i;
		} c;
		c.i = readU32();
		return c.f;
	}
	
	void readColor(cocos2d::ccColor4B& color)
	{
		unsigned int raw = readU32();
		color.a = static_cast<GLubyte>(raw >> 24);
		color.r = static_cast<GLubyte>(raw >> 16);
		color.g = static_cast<GLubyte>(raw >> 8);
		color.b = static_cast<GLubyte>(raw);
	}
	
	ss_offset readOffset()
	{
		return static_cast<ss_offset>(readS32());
	}

private:
	const ss_u16*	_dataPtr;
};


/**
 * CellRef
 */
struct CellRef
{
	const Cell* cell;
	cocos2d::CCTexture2D* texture;
	cocos2d::CCRect rect;
};


/**
 * CellCache
 */
class CellCache
{
public:
	static CellCache* create(const ProjectData* data, const std::string& imageBaseDir)
	{
		CellCache* obj = new CellCache();
		if (obj)
		{
			obj->init(data, imageBaseDir);
		}
		return obj;
	}

	CellRef* getReference(int index)
	{
		if (index < 0 || index >= _refs.size())
		{
			CCLOGERROR("Index out of range > %d", index);
			CC_ASSERT(0);
		}
		CellRef* ref = _refs.at(index);
		return ref;
	}

	//�w�肵�����O�̃Z���̎Q�ƃe�N�X�`����ύX����
	bool setCellRefTexture(const ProjectData* data, const char* cellName, cocos2d::CCTexture2D* texture)
	{
		bool rc = false;

		ToPointer ptr(data);
		const Cell* cells = static_cast<const Cell*>(ptr(data->cells));

		//���O����C���f�b�N�X�̎擾
		int cellindex = -1;
		for (int i = 0; i < data->numCells; i++)
		{
			const Cell* cell = &cells[i];
			const CellMap* cellMap = static_cast<const CellMap*>(ptr(cell->cellMap));
			const char* name = static_cast<const char*>(ptr(cellMap->name));
			if (std::strcmp(cellName, name) == 0)
			{
				CellRef* ref = getReference(i);
				ref->texture = texture;
				rc = true;
			}
		}

		return(rc);
	}

protected:
	void init(const ProjectData* data, const std::string& imageBaseDir)
	{
		CCAssert(data != NULL, "Invalid data");
		
		_textures.clear();
		_refs.clear();
		
		ToPointer ptr(data);
		const Cell* cells = static_cast<const Cell*>(ptr(data->cells));

		for (int i = 0; i < data->numCells; i++)
		{
			const Cell* cell = &cells[i];
			const CellMap* cellMap = static_cast<const CellMap*>(ptr(cell->cellMap));
			
			if (cellMap->index >= _textures.size())
			{
				const char* imagePath = static_cast<const char*>(ptr(cellMap->imagePath));
				addTexture(imagePath, imageBaseDir);
			}
			
			CellRef* ref = new CellRef();
			ref->cell = cell;
			ref->texture = _textures.at(cellMap->index);
			ref->rect = cocos2d::CCRect(cell->x, cell->y, cell->width, cell->height);
			_refs.push_back(ref);
		}
	}

	void addTexture(const std::string& imagePath, const std::string& imageBaseDir)
	{
		std::string path = "";
		
		cocos2d::CCFileUtils *fileUtils = cocos2d::CCFileUtils::sharedFileUtils();
		if (fileUtils->isAbsolutePath(imagePath))
		{
			// ��΃p�X�̂Ƃ��͂��̂܂܈���
			path = imagePath;
		}
		else
		{
			// ���΃p�X�̂Ƃ���imageBaseDir��t�^����
			path.append(imageBaseDir);
			size_t pathLen = path.length();
			if (pathLen && path.at(pathLen-1) != '/' && path.at(pathLen-1) != '\\')
			{
				path.append("/");
			}
			path.append(imagePath);
		}
		
		cocos2d::CCTextureCache* texCache = cocos2d::CCTextureCache::sharedTextureCache();
		cocos2d::CCTexture2D* tex = texCache->addImage(path.c_str());
		if (tex == NULL)
		{
			std::string msg = "Can't load image > " + path;
			CCAssert(tex != NULL, msg.c_str());
		}
		CCLOG("load: %s", path.c_str());
		_textures.push_back(tex);
	}

protected:
	std::vector<cocos2d::CCTexture2D*>	_textures;
	std::vector<CellRef*>				_refs;
};


/**
 * AnimeRef
 */
struct AnimeRef
{
	std::string				packName;
	std::string				animeName;
	const AnimationData*	animationData;
	const AnimePackData*	animePackData;
};


/**
 * AnimeCache
 */
class AnimeCache
{
public:
	static AnimeCache* create(const ProjectData* data)
	{
		AnimeCache* obj = new AnimeCache();
		if (obj)
		{
			obj->init(data);
		}
		return obj;
	}

	/**
	 * packName��animeName���w�肵��AnimeRef�𓾂�
	 */
	AnimeRef* getReference(const std::string& packName, const std::string& animeName)
	{
		std::string key = toPackAnimeKey(packName, animeName);
		AnimeRef* ref = _dic.at(key);
		return ref;
	}

	/**
	 * animeName�̂ݎw�肵��AnimeRef�𓾂�
	 */
	AnimeRef* getReference(const std::string& animeName)
	{
		AnimeRef* ref = _dic.at(animeName);
		return ref;
	}
	
	void dump()
	{
		std::map<std::string, AnimeRef*>::iterator it = _dic.begin();
		while (it != _dic.end())
		{
			CCLOG("%s", (*it).second);
			++it;
		}
	}

protected:
	void init(const ProjectData* data)
	{
		CCAssert(data != NULL, "Invalid data");
		
		ToPointer ptr(data);
		const AnimePackData* animePacks = static_cast<const AnimePackData*>(ptr(data->animePacks));

		for (int packIndex = 0; packIndex < data->numAnimePacks; packIndex++)
		{
			const AnimePackData* pack = &animePacks[packIndex];
			const AnimationData* animations = static_cast<const AnimationData*>(ptr(pack->animations));
			const char* packName = static_cast<const char*>(ptr(pack->name));
			
			for (int animeIndex = 0; animeIndex < pack->numAnimations; animeIndex++)
			{
				const AnimationData* anime = &animations[animeIndex];
				const char* animeName = static_cast<const char*>(ptr(anime->name));
				
				AnimeRef* ref = new AnimeRef();
				ref->packName = packName;
				ref->animeName = animeName;
				ref->animationData = anime;
				ref->animePackData = pack;

				// packName + animeName�ł̓o�^
				std::string key = toPackAnimeKey(packName, animeName);
				CCLOG("anime key: %s", key.c_str());
				_dic.insert(std::map<std::string, AnimeRef*>::value_type(key, ref));

				// animeName�݂̂ł̓o�^
				_dic.insert(std::map<std::string, AnimeRef*>::value_type(animeName, ref));
				
			}
		}
	}

	static std::string toPackAnimeKey(const std::string& packName, const std::string& animeName)
	{
		return Format("%s/%s", packName.c_str(), animeName.c_str());
	}
protected:
	std::map<std::string, AnimeRef*>	_dic;
};





/**
 * ResourceSet
 */
struct ResourceSet
{
	const ProjectData* data;
	bool isDataAutoRelease;
	CellCache* cellCache;
	AnimeCache* animeCache;

	virtual ~ResourceSet()
	{
		if (isDataAutoRelease)
		{
			delete data;
			data = NULL;
		}
		if (animeCache)
		{
			delete animeCache;
			animeCache = NULL;
		}
		if (cellCache)
		{
			delete cellCache;
			cellCache = NULL;
		}
	}
};


/**
 * ResourceManager
 */

static ResourceManager* defaultInstance = NULL;
const std::string ResourceManager::s_null;

ResourceManager* ResourceManager::getInstance()
{
	if (!defaultInstance)
	{
		defaultInstance = ResourceManager::create();
	}
	return defaultInstance;
}

ResourceManager::ResourceManager(void)
{
}

ResourceManager::~ResourceManager()
{
}

ResourceManager* ResourceManager::create()
{
	ResourceManager* obj = new ResourceManager();
	return obj;
}

ResourceSet* ResourceManager::getData(const std::string& dataKey)
{
	ResourceSet* rs = _dataDic.at(dataKey);
	return rs;
}

std::string ResourceManager::addData(const std::string& dataKey, const ProjectData* data, const std::string& imageBaseDir)
{
	CCAssert(data != NULL, "Invalid data");
	CCAssert(data->dataId == DATA_ID, "Not data id matched");
	CCAssert(data->version == DATA_VERSION, "Version number of data does not match");
	
	// imageBaseDir�̎w�肪�Ȃ��Ƃ��R���o�[�g���Ɏw�肳�ꂽ�p�X���g�p����
	std::string baseDir = imageBaseDir;
	if (imageBaseDir == s_null && data->imageBaseDir)
	{
		ToPointer ptr(data);
		const char* dir = static_cast<const char*>(ptr(data->imageBaseDir));
		baseDir = dir;
	}

	CellCache* cellCache = CellCache::create(data, baseDir);
	
	AnimeCache* animeCache = AnimeCache::create(data);
	
	ResourceSet* rs = new ResourceSet();
	rs->data = data;
	rs->isDataAutoRelease = false;
	rs->cellCache = cellCache;
	rs->animeCache = animeCache;
	_dataDic.insert(std::map<std::string, ResourceSet*>::value_type(dataKey, rs));

	return dataKey;
}

std::string ResourceManager::addDataWithKey(const std::string& dataKey, const std::string& ssbpFilepath, const std::string& imageBaseDir)
{
	cocos2d::CCFileUtils *fileUtils = cocos2d::CCFileUtils::sharedFileUtils();
	std::string fullpath = fileUtils->fullPathForFilename(ssbpFilepath.c_str());

	unsigned long nSize = 0;
	void* loadData = fileUtils->getFileData(fullpath.c_str(), "rb", &nSize);
	if (loadData == NULL)
	{
		std::string msg = "Can't load project data > " + fullpath;
		CCAssert(loadData != NULL, msg.c_str());
	}
	
	const ProjectData* data = static_cast<const ProjectData*>(loadData);
	CCAssert(data->dataId == DATA_ID, "Not data id matched");
	CCAssert(data->version == DATA_VERSION, "Version number of data does not match");
	
	std::string baseDir = imageBaseDir;
	if (imageBaseDir == s_null)
	{
		// imageBaseDir�̎w�肪�Ȃ��Ƃ�
		if (data->imageBaseDir)
		{
			// �R���o�[�g���Ɏw�肳�ꂽ�p�X���g�p����
			ToPointer ptr(data);
			const char* dir = static_cast<const char*>(ptr(data->imageBaseDir));
			baseDir = dir;
		}
		else
		{
			// �v���W�F�N�g�t�@�C���Ɠ����f�B���N�g�����w�肷��
			std::string directory;
			std::string filename;
			splitPath(directory, filename, ssbpFilepath);
			baseDir = directory;
		}
		//CCLOG("imageBaseDir: %s", baseDir.c_str());
	}

	addData(dataKey, data, baseDir);
	
	// ���\�[�X���j�������Ƃ��ꏏ�Ƀ��[�h�����f�[�^���j������
	ResourceSet* rs = getData(dataKey);
	CCAssert(rs != NULL, "");
	rs->isDataAutoRelease = true;
	
	return dataKey;
}

std::string ResourceManager::addData(const std::string& ssbpFilepath, const std::string& imageBaseDir)
{
	// �t�@�C���������o��
	std::string directory;
    std::string filename;
	splitPath(directory, filename, ssbpFilepath);
	
	// �g���q�����
	std::string dataKey = filename;
	size_t pos = filename.find_last_of(".");
    if (pos != std::string::npos)
    {
        dataKey = filename.substr(0, pos);
    }
	
	return addDataWithKey(dataKey, ssbpFilepath, imageBaseDir);
}

void ResourceManager::removeData(const std::string& dataKey)
{
	_dataDic.erase(dataKey);
}

void ResourceManager::removeAllData()
{
	_dataDic.clear();
}

//�f�[�^���A�Z�������w�肵�āA�Z���Ŏg�p���Ă���e�N�X�`����ύX����
bool ResourceManager::changeTexture(char* dataName, char* callName, cocos2d::CCTexture2D* texture)
{
	bool rc = false;

	ResourceSet* rs = getData(dataName);
	rc = rs->cellCache->setCellRefTexture(rs->data, callName, texture);

	return(rc);
}



/**
* State
*/
struct State
{
	float	x;
	float	y;
	float	rotationX;
	float	rotationY;
	float	rotationZ;
	float	scaleX;
	float	scaleY;
	cocos2d::CCAffineTransform	trans;

	void init()
	{
		x = 0;
		y = 0;
		rotationX = 0.0f;
		rotationY = 0.0f;
		rotationZ = 0.0f;
		scaleX = 1.0f;
		scaleY = 1.0f;
		trans = cocos2d::CCAffineTransformMakeIdentity();
	}

	State() { init(); }
};


/**
 * CustomSprite
 */
class CustomSprite : public cocos2d::CCSprite
{
private:
	static unsigned int ssSelectorLocation;
	static unsigned int	ssAlphaLocation;
	static unsigned int	sshasPremultipliedAlpha;

	static cocos2d::CCGLProgram* getCustomShaderProgram();

private:
	cocos2d::CCGLProgram*	_defaultShaderProgram;
	bool				_useCustomShaderProgram;
	float				_opacity;
	int					_hasPremultipliedAlpha;
	int					_colorBlendFuncNo;
	bool				_flipX;
	bool				_flipY;

public:
	State				_state;
	bool				_isStateChanged;
	CustomSprite*		_parent;
	ss::Player*			_ssplayer;
	float				_liveFrame;
	
public:
	CustomSprite();
	virtual ~CustomSprite();

	static CustomSprite* create();

	void initState()
	{
		_state.init();
		_isStateChanged = true;
	}
	
	void setStateValue(float& ref, float value)
	{
		if (ref != value)
		{
			ref = value;
			_isStateChanged = true;
		}
	}
	
	void setState(const State& state)
	{
		setStateValue(_state.x, state.x);
		setStateValue(_state.y, state.y);
		setStateValue(_state.rotationX, state.rotationX);
		setStateValue(_state.rotationY, state.rotationY);
		setStateValue(_state.rotationZ, state.rotationZ);
		setStateValue(_state.scaleX, state.scaleX);
		setStateValue(_state.scaleY, state.scaleY);
	}
	

	// override
	virtual void draw(void);
	virtual void setOpacity(GLubyte opacity);
	
	// original functions
	void changeShaderProgram(bool useCustomShaderProgram);
	bool isCustomShaderProgramEnabled() const;
	void setColorBlendFunc(int colorBlendFuncNo);
	cocos2d::ccV3F_C4B_T2F_Quad& getAttributeRef();

	void setFlippedX(bool flip);
	void setFlippedY(bool flip);
	bool isFlippedX();
	bool isFlippedY();
	void sethasPremultipliedAlpha(int PremultipliedAlpha);

public:
};



/**
 * Player
 */

static const std::string s_nullString;

Player::Player(void)
	: _resman(NULL)
	, _currentRs(NULL)
	, _currentAnimeRef(NULL)

	, _frameSkipEnabled(true)
	, _playingFrame(0.0f)
	, _step(1.0f)
	, _loop(0)
	, _loopCount(0)
	, _isPlaying(false)
	, _isPausing(false)
	, _prevDrawFrameNo(-1)
	, _InstanceAlpha(255)
	, _delegate(0)
	, _playEndTarget(NULL)
	, _playEndSelector(NULL)
{
	int i;
	for (i = 0; i < PART_VISIBLE_MAX; i++)
	{
		_partVisible[i] = true;
	}

}

Player::~Player()
{
	this->unscheduleUpdate();
	releaseParts();
	releaseData();
	releaseResourceManager();
	releaseAnime();
}

Player* Player::create(ResourceManager* resman)
{
	Player* obj = new Player();
	if (obj && obj->init())
	{
		obj->setResourceManager(resman);
		obj->autorelease();
		obj->scheduleUpdate();
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return NULL;
}

bool Player::init()
{
    if (!cocos2d::CCSprite::init())
    {
        return false;
    }
	return true;
}

void Player::releaseResourceManager()
{
//	if ( _resman )
//	{
//		delete _resman;
//		_resman = NULL;
//	}
}

void Player::setResourceManager(ResourceManager* resman)
{
	if (_resman) releaseResourceManager();
	
	if (!resman)
	{
		// null�̂Ƃ��̓f�t�H���g���g�p����
		resman = ResourceManager::getInstance();
	}
	
	_resman = resman;
}

int Player::getMaxFrame() const
{
	if (_currentAnimeRef )
	{
		return(_currentAnimeRef->animationData->numFrames);
	}
	else
	{
		return(0);
	}

}

int Player::getFrameNo() const
{
	return static_cast<int>(_playingFrame);
}

void Player::setFrameNo(int frameNo)
{
	_playingFrame = frameNo;
}

float Player::getStep() const
{
	return _step;
}

void Player::setStep(float step)
{
	_step = step;
}

int Player::getLoop() const
{
	return _loop;
}

void Player::setLoop(int loop)
{
	if (loop < 0) return;
	_loop = loop;
}

int Player::getLoopCount() const
{
	return _loopCount;
}

void Player::clearLoopCount()
{
	_loopCount = 0;
}

void Player::setFrameSkipEnabled(bool enabled)
{
	_frameSkipEnabled = enabled;
	_playingFrame = (int)_playingFrame;
}

bool Player::isFrameSkipEnabled() const
{
	return _frameSkipEnabled;
}
/*
void Player::setUserDataCallback(const UserDataCallback& callback)
{
	_userDataCallback = callback;
}

void Player::setPlayEndCallback(const PlayEndCallback& callback)
{
	_playEndCallback = callback;
}
*/

void Player::setData(const std::string& dataKey)
{
	ResourceSet* rs = _resman->getData(dataKey);
	_currentdataKey = dataKey;
	if (rs == NULL)
	{
		std::string msg = Format("Not found data > %s", dataKey.c_str());
		CCAssert(rs != NULL, msg.c_str());
	}
	
	if (_currentRs != rs)
	{
		releaseData();
		_currentRs = rs;
	}
}

void Player::releaseData()
{
	releaseAnime();
//	if (_currentRs )
//	{
//		delete _currentRs;
//		_currentRs = NULL;
//	}
}


void Player::releaseAnime()
{
	releaseParts();
//	if (_currentAnimeRef)
//	{
//		delete _currentAnimeRef;
//		_currentAnimeRef = NULL;
//	}
}

void Player::play(const std::string& packName, const std::string& animeName, int loop, int startFrameNo)
{
	CCAssert(_currentRs != NULL, "Not select data");
	
	AnimeRef* animeRef = _currentRs->animeCache->getReference(packName, animeName);
	if (animeRef == NULL)
	{
		std::string msg = Format("Not found animation > pack=%s, anime=%s", packName.c_str(), animeName.c_str());
		CCAssert(animeRef != NULL, msg.c_str());
	}

	play(animeRef, loop, startFrameNo);
}

void Player::play(const std::string& animeName, int loop, int startFrameNo)
{
	CCAssert(_currentRs != NULL, "Not select data");

	AnimeRef* animeRef = _currentRs->animeCache->getReference(animeName);
	if (animeRef == NULL)
	{
		std::string msg = Format("Not found animation > anime=%s", animeName.c_str());
		CCAssert(animeRef != NULL, msg.c_str());
	}

	play(animeRef, loop, startFrameNo);
}

void Player::play(AnimeRef* animeRef, int loop, int startFrameNo)
{
	if (_currentAnimeRef != animeRef)
	{
//		if (_currentAnimeRef)
//		{
//			delete _currentAnimeRef;
//			_currentAnimeRef = NULL;
//		}

		_currentAnimeRef = animeRef;
		
		allocParts(animeRef->animePackData->numParts, false);
		setPartsParentage();
	}
	_playingFrame = static_cast<float>(startFrameNo);
	_step = 1.0f;
	_loop = loop;
	_loopCount = 0;
	_isPlaying = true;
	_isPausing = false;
	_prevDrawFrameNo = -1;

	setFrame(_playingFrame);
}

void Player::pause()
{
	_isPausing = true;
}

void Player::resume()
{
	_isPausing = false;
}

void Player::stop()
{
	_isPlaying = false;
}

const std::string& Player::getPlayPackName() const
{
	return _currentAnimeRef != NULL ? _currentAnimeRef->packName : s_nullString;
}

const std::string& Player::getPlayAnimeName() const
{
	return _currentAnimeRef != NULL ? _currentAnimeRef->animeName : s_nullString;
}

void Player::setDelegate(SSPlayerDelegate* delegate)
{
	_delegate = delegate;
}

void Player::update(float dt)
{
	updateFrame(dt);
}

void Player::updateFrame(float dt)
{
	if (!_currentAnimeRef) return;
	
	bool playEnd = false;
	bool toNextFrame = _isPlaying && !_isPausing;
	if (toNextFrame && (_loop == 0 || _loopCount < _loop))
	{
		// �t���[����i�߂�.
		// forward frame.
		const int numFrames = _currentAnimeRef->animationData->numFrames;

		float fdt = _frameSkipEnabled ? dt : cocos2d::CCDirector::sharedDirector()->getAnimationInterval();
		float s = fdt / (1.0f / _currentAnimeRef->animationData->fps);
		
		//if (!m_frameSkipEnabled) CCLOG("%f", s);
		
		float next = _playingFrame + (s * _step);

		int nextFrameNo = static_cast<int>(next);
		float nextFrameDecimal = next - static_cast<float>(nextFrameNo);
		int currentFrameNo = static_cast<int>(_playingFrame);
		
		if (_step >= 0)
		{
			// ���Đ���.
			// normal plays.
			for (int c = nextFrameNo - currentFrameNo; c; c--)
			{
				int incFrameNo = currentFrameNo + 1;
				if (incFrameNo >= numFrames)
				{
					// �A�j�����ꏄ
					// turned animation.
					_loopCount += 1;
					if (_loop && _loopCount >= _loop)
					{
						// �Đ��I��.
						// play end.
						playEnd = true;
						break;
					}
					
					incFrameNo = 0;
				}
				currentFrameNo = incFrameNo;

				// ���̃t���[���̃��[�U�[�f�[�^���`�F�b�N
				// check the user data of this frame.
				checkUserData(currentFrameNo);
			}
		}
		else
		{
			// �t�Đ���.
			// reverse play.
			for (int c = currentFrameNo - nextFrameNo; c; c--)
			{
				int decFrameNo = currentFrameNo - 1;
				if (decFrameNo < 0)
				{
					// �A�j�����ꏄ
					// turned animation.
					_loopCount += 1;
					if (_loop && _loopCount >= _loop)
					{
						// �Đ��I��.
						// play end.
						playEnd = true;
						break;
					}
				
					decFrameNo = numFrames - 1;
				}
				currentFrameNo = decFrameNo;
				
				// ���̃t���[���̃��[�U�[�f�[�^���`�F�b�N
				// check the user data of this frame.
				checkUserData(currentFrameNo);
			}
		}
		
		_playingFrame = static_cast<float>(currentFrameNo) + nextFrameDecimal;
	}
	else
	{
		//�A�j�����蓮�ōX�V����ꍇ
		checkUserData(getFrameNo());
	}

	setFrame(getFrameNo());
	
	if (playEnd)
	{
		stop();
	
		// �Đ��I���R�[���o�b�N�̌Ăяo��
        if (_playEndTarget)
        {
            (_playEndTarget->*_playEndSelector)(this);
        }
	}
}




void Player::allocParts(int numParts, bool useCustomShaderProgram)
{
	if (getChildrenCount() < numParts)
	{
		// �p�[�c������CustomSprite���쐬����
//		// create CustomSprite objects.
		for (int i = getChildrenCount(); i < numParts; i++)
		{
			CustomSprite* sprite =  CustomSprite::create();
			sprite->changeShaderProgram(useCustomShaderProgram);
			
			_parts.push_back(sprite);
			addChild(sprite);
		}
	}
	else
	{
		// �������͉������
		for (int i = getChildrenCount() - 1; i >= numParts; i--)
		{
			CCObject* child = m_pChildren->objectAtIndex(i);
			CustomSprite* sprite = (CustomSprite*)child;
			removeChild(sprite, true);

			for (std::vector<CustomSprite *>::iterator it = _parts.begin(); it != _parts.end();)
			{
				if (*it == sprite)
				{ 
					// �폜�����ɍ����v�f���폜����
					it = _parts.erase(it);
				}
				else{
					++it;
				}
			}
		}
	
		// �p�����[�^������
		for (int i = 0; i < numParts; i++)
		{
			CCObject* child = m_pChildren->objectAtIndex(i);
			CustomSprite* sprite = (CustomSprite*)child;
			sprite->initState();
		}
	}

	if (m_pChildren && m_pChildren->count() > 0)
	{
		CCObject* child;
		CCARRAY_FOREACH(m_pChildren, child)
		{
			CustomSprite* sprite = (CustomSprite*)child;
			sprite->setVisible(false);
		}
	}

}

void Player::releaseParts()
{
	// �p�[�c�̎qCustomSprite��S�č폜
	// remove children CCSprite objects.
	removeAllChildrenWithCleanup(true);
	_parts.clear();
}

void Player::setPartsParentage()
{
	if (!_currentAnimeRef) return;

	ToPointer ptr(_currentRs->data);
	const AnimePackData* packData = _currentAnimeRef->animePackData;
	const PartData* parts = static_cast<const PartData*>(ptr(packData->parts));

	//�e�q�֌W��ݒ�
	for (int partIndex = 0; partIndex < packData->numParts; partIndex++)
	{
		const PartData* partData = &parts[partIndex];
		CustomSprite* sprite = static_cast<CustomSprite*>(_parts.at(partIndex));
		
		if (partIndex > 0)
		{
			CustomSprite* parent = static_cast<CustomSprite*>(_parts.at(partData->parentIndex));
			sprite->_parent = parent;
		}
		else
		{
			sprite->_parent = NULL;
		}

		//�C���X�^���X�p�[�c�̐���
		sprite->removeAllChildrenWithCleanup(true);	//�q���̃p�[�c���폜

		std::string refanimeName = static_cast<const char*>(ptr(partData->refname));
		if (refanimeName != "")
		{
			//�C���X�^���X�p�[�c���ݒ肳��Ă���
			sprite->_ssplayer = ss::Player::create();
			sprite->_ssplayer->setData(_currentdataKey);
			sprite->_ssplayer->play(refanimeName);				 // �A�j���[�V���������w��(ssae��/�A�j���[�V���������\�A�ڂ����͌�q)
			sprite->_ssplayer->stop();
			sprite->addChild(sprite->_ssplayer);
		}
	}
}

//index����p�[�c�����擾
const char* Player::getPartName(int partId) const
{
	ToPointer ptr(_currentRs->data);

	const AnimePackData* packData = _currentAnimeRef->animePackData;
	CCAssert(partId >= 0 && partId < packData->numParts, "partId is out of range.");

	const PartData* partData = static_cast<const PartData*>(ptr(packData->parts));
	const char* name = static_cast<const char*>(ptr(partData[partId].name));
	return name;
}

//�p�[�c������index���擾
int Player::indexOfPart(const char* partName) const
{
	const AnimePackData* packData = _currentAnimeRef->animePackData;
	for (int i = 0; i < packData->numParts; i++)
	{
		const char* name = getPartName(i);
		if (std::strcmp(partName, name) == 0)
		{
			return i;
		}
	}
	return -1;
}

/*
 �p�[�c������w��t���[���̃p�[�c�X�e�[�^�X���擾���܂��B
 �K�v�ɉ����ā@ResluteState�@��ҏW���f�[�^���擾���Ă��������B

 �p�[�c�̍��W���擾����ꍇ�A�Ώۂ̃p�[�c��root�p�[�c�̒����ɔz�u���Ă��������B
 �e�p�[�c���ړ������ꍇ�A�q���p�[�c�����킹�Ĉړ����܂����A�q���̈ړ��ʂ̓f�[�^�Ƃ��ďo�͂���ĂȂ����ߍX�V����܂���B
 ��ʂɕ\������Ă�����W�ł͂Ȃ��uSS5��ŃA�g���r���[�g�ɕ\������Ă�����W�v���擾�ł���`�ɂȂ�܂��B
*/
bool Player::getPartState(ResluteState& result, const char* name, int frameNo)
{
	if (_currentAnimeRef)
	{
		{
			//�J�����g�t���[���̃p�[�c�X�e�[�^�X���擾����
			if (frameNo = -1)
			{
				//�t���[���̎w�肪�ȗ����ꂽ�ꍇ�͌��݂̃t���[�����g�p����
				frameNo = getFrameNo();
			}

			ToPointer ptr(_currentRs->data);

			const AnimePackData* packData = _currentAnimeRef->animePackData;
			const PartData* parts = static_cast<const PartData*>(ptr(packData->parts));

			const AnimationData* animeData = _currentAnimeRef->animationData;
			const ss_offset* frameDataIndex = static_cast<const ss_offset*>(ptr(animeData->frameData));

			const ss_u16* frameDataArray = static_cast<const ss_u16*>(ptr(frameDataIndex[frameNo]));
			DataArrayReader reader(frameDataArray);

			const AnimationInitialData* initialDataList = static_cast<const AnimationInitialData*>(ptr(animeData->defaultData));


			cocos2d::ccV3F_C4B_T2F_Quad tempQuad;

			/*
			ssbp�ɂ͕`�揇���\�[�g���ꂽ��ԂŃf�[�^���܂܂�Ă���A���A�e�p�[�c�̃f�[�^�͉ϒ��ł��邽��
			�Y���̃p�[�c����������̂Ƀf�[�^��ǂݐi�߂Ă����K�v������B
			�p�[�c���������鎖�Ō����̃R�X�g���啝�ɑ�����Ǝv����B
			���t���[���X�e�[�^�X���擾����ꍇ�͂��炩���ߌ�������p�[�c�̗D��x��Ⴍ���邱�Ƃőf�����������鎖���ł���B
			*/
			for (int index = 0; index < packData->numParts; index++)
			{
				int partIndex = reader.readS16();
				const PartData* partData = &parts[partIndex];
				const AnimationInitialData* init = &initialDataList[partIndex];

				// optional parameters
				int flags = reader.readU32();																			//��\���A���]�t���O
				int cellIndex = flags & PART_FLAG_CELL_INDEX ? reader.readS16() : init->cellIndex;						//�Z���̔ԍ�
				float x = flags & PART_FLAG_POSITION_X ? reader.readS16() : init->positionX;							//X���W
				float y = flags & PART_FLAG_POSITION_Y ? reader.readS16() : init->positionY;							//Y���W
				float anchorX = flags & PART_FLAG_ANCHOR_X ? reader.readFloat() : init->anchorX;						//���_�I�t�Z�b�g
				float anchorY = flags & PART_FLAG_ANCHOR_Y ? reader.readFloat() : init->anchorY;						//���_�I�t�Z�b�g
				float rotationX = flags & PART_FLAG_ROTATIONX ? -reader.readFloat() : -init->rotationX;					//X��]
				float rotationY = flags & PART_FLAG_ROTATIONY ? -reader.readFloat() : -init->rotationY;					//Y��]
				float rotationZ = flags & PART_FLAG_ROTATIONZ ? -reader.readFloat() : -init->rotationZ;					//Z��]
				float scaleX = flags & PART_FLAG_SCALE_X ? reader.readFloat() : init->scaleX;							//X�g�嗦
				float scaleY = flags & PART_FLAG_SCALE_Y ? reader.readFloat() : init->scaleY;							//Y�g�嗦
				int opacity = flags & PART_FLAG_OPACITY ? reader.readU16() : init->opacity;								//�����x
				float size_x = flags & PART_FLAG_SIZE_X ? reader.readFloat() : init->size_X;							//X�T�C�Y
				float size_y = flags & PART_FLAG_SIZE_Y ? reader.readFloat() : init->size_Y;							//Y�T�C�Y
				float uv_move_X = flags & PART_FLAG_U_MOVE ? reader.readFloat() : init->uv_move_X;						//UVX�ړ�
				float uv_move_Y = flags & PART_FLAG_V_MOVE ? reader.readFloat() : init->uv_move_Y;						//UVY�ړ�
				float uv_rotation = flags & PART_FLAG_UV_ROTATION ? reader.readFloat() : init->uv_rotation;				//UV��]
				float uv_scale_X = flags & PART_FLAG_U_SCALE ? reader.readFloat() : init->uv_scale_X;					//UVX�X�P�[��
				float uv_scale_Y = flags & PART_FLAG_V_SCALE ? reader.readFloat() : init->uv_scale_Y;					//UVY�X�P�[��
				float boundingRadius = flags & PART_FLAG_BOUNDINGRADIUS ? reader.readFloat() : init->boundingRadius;	//�����蔼�a

				bool isVisibled = !(flags & PART_FLAG_INVISIBLE);

				//�Œ菭���������֖߂�
				x = x / DOT;
				y = y / DOT;

				// ���_�ό`�̃I�t�Z�b�g�l�𔽉f
				if (flags & PART_FLAG_VERTEX_TRANSFORM)
				{
					int vt_flags = reader.readU16();
					if (vt_flags & VERTEX_FLAG_LT)
					{
						reader.readS16();
						reader.readS16();
					}
					if (vt_flags & VERTEX_FLAG_RT)
					{
						reader.readS16();
						reader.readS16();
					}
					if (vt_flags & VERTEX_FLAG_LB)
					{
						reader.readS16();
						reader.readS16();
					}
					if (vt_flags & VERTEX_FLAG_RB)
					{
						reader.readS16();
						reader.readS16();
					}
				}

				// �J���[�u�����h�̔��f
				cocos2d::ccColor4B color4 = { 0xff, 0xff, 0xff, 0xff };
				if (flags & PART_FLAG_COLOR_BLEND)
				{

					int typeAndFlags = reader.readU16();
					int funcNo = typeAndFlags & 0xff;
					int cb_flags = (typeAndFlags >> 8) & 0xff;
					float blend_rate = 1.0f;

					if (cb_flags & VERTEX_FLAG_ONE)
					{
						blend_rate = reader.readFloat();
						reader.readColor(color4);
					}
					else
					{
						if (cb_flags & VERTEX_FLAG_LT)
						{
							blend_rate = reader.readFloat();
							reader.readColor(color4);
						}
						if (cb_flags & VERTEX_FLAG_RT)
						{
							blend_rate = reader.readFloat();
							reader.readColor(color4);
						}
						if (cb_flags & VERTEX_FLAG_LB)
						{
							blend_rate = reader.readFloat();
							reader.readColor(color4);
						}
						if (cb_flags & VERTEX_FLAG_RB)
						{
							blend_rate = reader.readFloat();
							reader.readColor(color4);
						}
					}
				}

				//�C���X�^���X�p�[�c�̏ꍇ
				if (partData->type == PARTTYPE_INSTANCE)
				{
					//�`��
					int refKeyframe = 0;
					int refStartframe = 0;
					int refEndframe = 0;
					float refSpeed = 0;
					int refloopNum = 0;
					bool infinity = false;
					bool reverse = false;
					bool pingpong = false;
					bool independent = false;

					if (flags & PART_FLAG_INSTANCE_KEYFRAME)
					{
						refKeyframe = reader.readS16();
					}
					if (flags & PART_FLAG_INSTANCE_START)
					{
						refStartframe = reader.readS16();
					}
					if (flags & PART_FLAG_INSTANCE_END)
					{
						refEndframe = reader.readS16();
					}
					if (flags & PART_FLAG_INSTANCE_SPEED)
					{
						refSpeed = reader.readFloat();
					}
					if (flags & PART_FLAG_INSTANCE_LOOP)
					{
						refloopNum = reader.readS16();
					}
					if (flags & PART_FLAG_INSTANCE_LOOP_FLG)
					{
						int lflags = reader.readS16();
						if (lflags & INSTANCE_LOOP_FLAG_INFINITY)
						{
							//�������[�v
							infinity = true;
						}
						if (lflags & INSTANCE_LOOP_FLAG_REVERSE)
						{
							//�t�Đ�
							reverse = true;
						}
						if (lflags & INSTANCE_LOOP_FLAG_PINGPONG)
						{
							//����
							pingpong = true;
						}
						if (lflags & INSTANCE_LOOP_FLAG_INDEPENDENT)
						{
							//�Ɨ�
							independent = true;
						}
					}
				}

				//�����p�[�c���̏ꍇ�Ɏ擾�������ݒ肵�ď����I��
				const char* partName = static_cast<const char*>(ptr(partData->name));
				if (std::strcmp(partName, name) == 0)
				{
					//�K�v�ɉ����Ď擾����p�����[�^��ǉ����Ă��������B
					//�����蔻��Ȃǂ̃p�[�c�ɕt������t���O���擾����ꍇ�́@partData�@�̃����o���Q�Ƃ��Ă��������B
					result.x = x;
					result.y = y;

					return true;
				}
			}

		}
	}
	return false;
}


//���x�������烉�x���̐ݒ肳��Ă���t���[�����擾
//���x�������݂��Ȃ��ꍇ�͖߂�l��-1�ƂȂ�܂��B
//���x�������S�p�ł��Ă���Ǝ擾�Ɏ��s���܂��B
int Player::getLabelToFrame(char* findLabelName)
{
	int rc = -1;

	ToPointer ptr(_currentRs->data);
	const AnimationData* animeData = _currentAnimeRef->animationData;

	if (!animeData->labelData) return -1;
	const ss_offset* labelDataIndex = static_cast<const ss_offset*>(ptr(animeData->labelData));


	int idx = 0;
	for (idx = 0; idx < animeData->labelNum; idx++ )
	{
		if (!labelDataIndex[idx]) return -1;
		const ss_u16* labelDataArray = static_cast<const ss_u16*>(ptr(labelDataIndex[idx]));

		DataArrayReader reader(labelDataArray);

		LabelData ldata;
		ss_offset offset = reader.readOffset();
		const char* str = static_cast<const char*>(ptr(offset));
		int labelFrame = reader.readU16();
		ldata.str = str;
		ldata.frameNo = labelFrame;

		if (ldata.str.compare(findLabelName) == 0 )
		{
			//�������O�̃��x������������
			return (ldata.frameNo);
		}
	}

	return (rc);
}

//����p�[�c�̕\���A��\����ݒ肵�܂�
//�p�[�c�ԍ��̓X�v���C�g�X�^�W�I�̃t���[���R���g���[���ɔz�u���ꂽ�p�[�c��
//�v���C�I���e�B�Ń\�[�g���ꂽ��A��ɔz�u���ꂽ���Ƀ\�[�g����Č��肳��܂��B
void Player::setPartVisible(int partNo, bool flg)
{
	_partVisible[partNo] = flg;
}


void Player::setFrame(int frameNo)
{
	if (!_currentAnimeRef) return;

	bool forceUpdate = false;
	{
		// �t���b�v�ɕω����������Ƃ��͕K���`����X�V����
		CustomSprite* root = static_cast<CustomSprite*>(_parts.at(0));
		float scaleX = root->isFlippedX() ? -1.0f : 1.0f;
		float scaleY = root->isFlippedY() ? -1.0f : 1.0f;
		root->setStateValue(root->_state.x, scaleX);
		root->setStateValue(root->_state.y, scaleY);
		forceUpdate = root->_isStateChanged;
	}
	
	// �O��̕`��t���[���Ɠ����Ƃ��̓X�L�b�v
	if (!forceUpdate && frameNo == _prevDrawFrameNo) return;

	_prevDrawFrameNo = frameNo;


	ToPointer ptr(_currentRs->data);

	const AnimePackData* packData = _currentAnimeRef->animePackData;
	const PartData* parts = static_cast<const PartData*>(ptr(packData->parts));

	const AnimationData* animeData = _currentAnimeRef->animationData;
	const ss_offset* frameDataIndex = static_cast<const ss_offset*>(ptr(animeData->frameData));
	
	const ss_u16* frameDataArray = static_cast<const ss_u16*>(ptr(frameDataIndex[frameNo]));
	DataArrayReader reader(frameDataArray);
	
	const AnimationInitialData* initialDataList = static_cast<const AnimationInitialData*>(ptr(animeData->defaultData));


	State state;
	cocos2d::ccV3F_C4B_T2F_Quad tempQuad;

	for (int index = 0; index < packData->numParts; index++)
	{
		int partIndex = reader.readS16();
		const PartData* partData = &parts[partIndex];
		const AnimationInitialData* init = &initialDataList[partIndex];

		// optional parameters
		int flags      = reader.readU32();
		int cellIndex  = flags & PART_FLAG_CELL_INDEX ? reader.readS16() : init->cellIndex;
		float x        = flags & PART_FLAG_POSITION_X ? reader.readS16() : init->positionX;
		float y        = flags & PART_FLAG_POSITION_Y ? reader.readS16() : init->positionY;
		float anchorX  = flags & PART_FLAG_ANCHOR_X ? reader.readFloat() : init->anchorX;
		float anchorY  = flags & PART_FLAG_ANCHOR_Y ? reader.readFloat() : init->anchorY;
		float rotationX = flags & PART_FLAG_ROTATIONX ? -reader.readFloat() : -init->rotationX;	//cocos2dx ver2�n�ł͔�Ή�
		float rotationY = flags & PART_FLAG_ROTATIONY ? -reader.readFloat() : -init->rotationY;	//cocos2dx ver2�n�ł͔�Ή�
		float rotationZ = flags & PART_FLAG_ROTATIONZ ? -reader.readFloat() : -init->rotationZ;
		float scaleX   = flags & PART_FLAG_SCALE_X ? reader.readFloat() : init->scaleX;
		float scaleY   = flags & PART_FLAG_SCALE_Y ? reader.readFloat() : init->scaleY;
		int opacity    = flags & PART_FLAG_OPACITY ? reader.readU16() : init->opacity;
		float size_x   = flags & PART_FLAG_SIZE_X ? reader.readFloat() : init->size_X;
		float size_y   = flags & PART_FLAG_SIZE_Y ? reader.readFloat() : init->size_Y;
		float uv_move_X   = flags & PART_FLAG_U_MOVE ? reader.readFloat() : init->uv_move_X;
		float uv_move_Y   = flags & PART_FLAG_V_MOVE ? reader.readFloat() : init->uv_move_Y;
		float uv_rotation = flags & PART_FLAG_UV_ROTATION ? reader.readFloat() : init->uv_rotation;
		float uv_scale_X  = flags & PART_FLAG_U_SCALE ? reader.readFloat() : init->uv_scale_X;
		float uv_scale_Y  = flags & PART_FLAG_V_SCALE ? reader.readFloat() : init->uv_scale_Y;
		float boundingRadius = flags & PART_FLAG_BOUNDINGRADIUS ? reader.readFloat() : init->boundingRadius;

		bool isVisibled = !(flags & PART_FLAG_INVISIBLE);

		if (_partVisible[index] == false)
		{
			//���[�U�[���C�ӂɔ�\���Ƃ����p�[�c�͔�\���ɐݒ�
			isVisibled = false;
		}

		//�Œ菭���������֖߂�
		x = x / DOT;
		y = y / DOT;

		//�C���X�^���X�p�[�c�̃p�����[�^��������
		//�s�����x�͂��łɃR���o�[�^�Őe�̓����x���v�Z����Ă��邽��
		//�S�p�[�c�ɃC���X�^���X�̓����x��������K�v������
		opacity = (opacity * _InstanceAlpha) / 255;

		//�X�e�[�^�X�ۑ�
		state.x = x;
		state.y = y;
		state.rotationX = rotationX;
		state.rotationY = rotationY;
		state.rotationZ = rotationZ;
		state.scaleX = scaleX;
		state.scaleY = scaleY;

		CustomSprite* sprite = static_cast<CustomSprite*>(_parts.at(partIndex));

		//���]
		//cocos2dx ver2.x�n�ɂ͔��]���Ȃ��̂ŁA����UV�����ւ��Ĕ��]����������
		//�X�P�[����-�ɂ��Ĕ��]���s���ƌ��_���ړ����Ă��܂�����UV�ōs��
		sprite->setFlippedX(flags & PART_FLAG_FLIP_H);
		sprite->setFlippedY(flags & PART_FLAG_FLIP_V);

		//�\���ݒ�
		sprite->setVisible(isVisibled);
		sprite->setState(state);
		this->reorderChild(sprite, index);

		sprite->setPosition(cocos2d::CCPoint(x, y));
		sprite->setRotation(rotationZ);

		CellRef* cellRef = cellIndex >= 0 ? _currentRs->cellCache->getReference(cellIndex) : NULL;
		bool setBlendEnabled = true;
		if (cellRef)
		{
			if (setBlendEnabled)
			{
				// �u�����h���@��ݒ�
				// �W����Ԃ�MIX�u�����h�����ɂȂ�܂�
				// BlendFunc�̒l��ύX���邱�ƂŃu�����h���@��؂�ւ��܂�
				cocos2d::ccBlendFunc blendFunc = sprite->getBlendFunc();

				if (flags & PART_FLAG_COLOR_BLEND)
				{
					//�J���[�u�����h���s���Ƃ��̓J�X�^���V�F�[�_�[���g�p����
					sprite->changeShaderProgram(true);

					if (!cellRef->texture->hasPremultipliedAlpha())
					{
						blendFunc.src = GL_SRC_ALPHA;
						blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
					}
					else
					{
						blendFunc.src = CC_BLEND_SRC;
						blendFunc.dst = CC_BLEND_DST;
					}

					// �J�X�^���V�F�[�_���g�p����ꍇ
					blendFunc.src = GL_SRC_ALPHA;
					
					// ���Z�u�����h
					if (partData->alphaBlendType == BLEND_ADD) {
						blendFunc.dst = GL_ONE;
					}
				}
				else
				{
					sprite->changeShaderProgram(false);
					// �ʏ�u�����h
					if (partData->alphaBlendType == BLEND_MIX)
					{
						if (opacity < 255)
						{
							if (!cellRef->texture->hasPremultipliedAlpha())
							{
								blendFunc.src = GL_SRC_ALPHA;
								blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
							}
							else
							{
								blendFunc.src = GL_ONE;
								blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
							}
						}
						else
						{
							blendFunc.src = GL_ONE;
							blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
						}
					}
					// ���Z�u�����h
					if (partData->alphaBlendType == BLEND_ADD) {
						blendFunc.src = GL_SRC_ALPHA;
						blendFunc.dst = GL_ONE;
					}
					// ��Z�u�����h
					if (partData->alphaBlendType == BLEND_MUL) {
						blendFunc.src = GL_DST_COLOR;
						blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
					}
					// ���Z�u�����h
					if (partData->alphaBlendType == BLEND_SUB) {
						blendFunc.src = GL_ONE_MINUS_SRC_ALPHA;
						blendFunc.dst = GL_ONE_MINUS_SRC_COLOR;
					}
					/*
					//���O
					if (partData->alphaBlendType == BLEND_) {
					blendFunc.src = GL_ONE_MINUS_DST_COLOR;
					blendFunc.dst = GL_ONE_MINUS_SRC_COLOR;
					}
					//�X�N���[��
					if (partData->alphaBlendType == BLEND_) {
					blendFunc.src = GL_ONE_MINUS_DST_COLOR;
					blendFunc.dst = GL_ONE;
					}
					*/
				}

				sprite->setBlendFunc(blendFunc);
			}

			sprite->setTexture(cellRef->texture);
			sprite->setTextureRect(cellRef->rect);
		}
		else
		{
			sprite->setTexture(NULL);
			sprite->setTextureRect(cocos2d::CCRect());
		}

		sprite->setAnchorPoint(cocos2d::CCPoint(anchorX , 1.0f - anchorY));	//cocos�͉���-�Ȃ̂ō��W�𔽓]������
//		sprite->setFlippedX(flags & PART_FLAG_FLIP_H);
//		sprite->setFlippedY(flags & PART_FLAG_FLIP_V);
		sprite->setOpacity(opacity);

		//���_�f�[�^�̎擾
		cocos2d::ccV3F_C4B_T2F_Quad& quad = sprite->getAttributeRef();

		//�T�C�Y�ݒ�
		if (flags & PART_FLAG_SIZE_X)
		{
			float w = 0;
			float center = 0;
			w = (quad.tr.vertices.x - quad.tl.vertices.x) / 2.0f;
			if (w!= 0.0f)
			{
				center = quad.tl.vertices.x + w;
				float scale = (size_x / 2.0f) / w;

				quad.bl.vertices.x = center - (w * scale);
				quad.br.vertices.x = center + (w * scale);
				quad.tl.vertices.x = center - (w * scale);
				quad.tr.vertices.x = center + (w * scale);
			}
		}
		if (flags & PART_FLAG_SIZE_Y)
		{
			float h = 0;
			float center = 0;
			h = (quad.bl.vertices.y - quad.tl.vertices.y) / 2.0f;
			if (h != 0.0f)
			{
				center = quad.tl.vertices.y + h;
				float scale = (size_y / 2.0f) / h;

				quad.bl.vertices.y = center - (h * scale);
				quad.br.vertices.y = center - (h * scale);
				quad.tl.vertices.y = center + (h * scale);
				quad.tr.vertices.y = center + (h * scale);
			}
		}
		//�X�P�[���ݒ�
		sprite->setScaleX(scaleX);
		sprite->setScaleY(scaleY);

		// ���_�ό`�̃I�t�Z�b�g�l�𔽉f
		if (flags & PART_FLAG_VERTEX_TRANSFORM)
		{
			int vt_flags = reader.readU16();
			if (vt_flags & VERTEX_FLAG_LT)
			{
				quad.tl.vertices.x += reader.readS16();
				quad.tl.vertices.y += reader.readS16();
			}
			if (vt_flags & VERTEX_FLAG_RT)
			{
				quad.tr.vertices.x += reader.readS16();
				quad.tr.vertices.y += reader.readS16();
			}
			if (vt_flags & VERTEX_FLAG_LB)
			{
				quad.bl.vertices.x += reader.readS16();
				quad.bl.vertices.y += reader.readS16();
			}
			if (vt_flags & VERTEX_FLAG_RB)
			{
				quad.br.vertices.x += reader.readS16();
				quad.br.vertices.y += reader.readS16();
			}
		}
		
		
		//���_���̎擾
		GLubyte alpha = (GLubyte)opacity;
		cocos2d::ccColor4B color4 = { 0xff, 0xff, 0xff, alpha };

		sprite->sethasPremultipliedAlpha(0);	//
		if (cellRef)
		{
			if (cellRef->texture->hasPremultipliedAlpha())
			{
				//�e�N�X�`���̃J���[�l�ɃA���t�@���������Ă���ꍇ�́A�A���t�@�l���J���[�l�ɔ��f������
				color4.r = color4.r * alpha / 255;
				color4.g = color4.g * alpha / 255;
				color4.b = color4.b * alpha / 255;
				sprite->sethasPremultipliedAlpha(1);
			}
		}
		quad.tl.colors =
		quad.tr.colors =
		quad.bl.colors =
		quad.br.colors = color4;


		// �J���[�u�����h�̔��f
		if (flags & PART_FLAG_COLOR_BLEND)
		{

			int typeAndFlags = reader.readU16();
			int funcNo = typeAndFlags & 0xff;
			int cb_flags = (typeAndFlags >> 8) & 0xff;
			float blend_rate = 1.0f;

			sprite->setColorBlendFunc(funcNo);
			
			if (cb_flags & VERTEX_FLAG_ONE)
			{
				blend_rate = reader.readFloat();
				reader.readColor(color4);

				color4.a = (int)( blend_rate * 255 );	//���[�g���A���t�@�l�Ƃ��Đݒ�
				quad.tl.colors =
				quad.tr.colors =
				quad.bl.colors =
				quad.br.colors = color4;
			}
			else
			{
				if (cb_flags & VERTEX_FLAG_LT)
				{
					blend_rate = reader.readFloat();
					reader.readColor(color4);
					color4.a = (int)(blend_rate * 255);	//���[�g���A���t�@�l�Ƃ��Đݒ�
					quad.tl.colors = color4;
				}
				if (cb_flags & VERTEX_FLAG_RT)
				{
					blend_rate = reader.readFloat();
					reader.readColor(color4);
					color4.a = (int)(blend_rate * 255);	//���[�g���A���t�@�l�Ƃ��Đݒ�
					quad.tr.colors = color4;
				}
				if (cb_flags & VERTEX_FLAG_LB)
				{
					blend_rate = reader.readFloat();
					reader.readColor(color4);
					color4.a = (int)(blend_rate * 255);	//���[�g���A���t�@�l�Ƃ��Đݒ�
					quad.bl.colors = color4;
				}
				if (cb_flags & VERTEX_FLAG_RB)
				{
					blend_rate = reader.readFloat();
					reader.readColor(color4);
					color4.a = (int)(blend_rate * 255);	//���[�g���A���t�@�l�Ƃ��Đݒ�
					quad.br.colors = color4;
				}
			}
		}
		//uv�X�N���[��
		{
			quad.tl.texCoords.u += uv_move_X;
			quad.tr.texCoords.u += uv_move_X;
			quad.bl.texCoords.u += uv_move_X;
			quad.br.texCoords.u += uv_move_X;
		}
		{
			quad.tl.texCoords.v += uv_move_Y;
			quad.tr.texCoords.v += uv_move_Y;
			quad.bl.texCoords.v += uv_move_Y;
			quad.br.texCoords.v += uv_move_Y;
		}


		float u_wide = 0;
		float v_height = 0;
		float u_center = 0;
		float v_center = 0;
		float u_code = 1;
		float v_code = 1;

		//cocos2v �̏ꍇ�͂����Ŕ��]���s��
		u_wide = (quad.tr.texCoords.u - quad.tl.texCoords.u) / 2.0f;
		u_center = quad.tl.texCoords.u + u_wide;
		if (flags & PART_FLAG_FLIP_H)
		{
			//���E���]���s���ꍇ�͕������t�ɂ���
			u_code = -1;
		}
		v_height = (quad.bl.texCoords.v - quad.tl.texCoords.v) / 2.0f;
		v_center = quad.tl.texCoords.v + v_height;
		if (flags & PART_FLAG_FLIP_V)
		{
			//�㉺���]���s���ꍇ�̓e�N�X�`��UV���t�ɂ���
			v_code = -1;
		}
		//UV��]
		if (flags & PART_FLAG_UV_ROTATION)
		{
			//���_�ʒu����]������
			get_uv_rotation(&quad.tl.texCoords.u, &quad.tl.texCoords.v, u_center, v_center, uv_rotation);
			get_uv_rotation(&quad.tr.texCoords.u, &quad.tr.texCoords.v, u_center, v_center, uv_rotation);
			get_uv_rotation(&quad.bl.texCoords.u, &quad.bl.texCoords.v, u_center, v_center, uv_rotation);
			get_uv_rotation(&quad.br.texCoords.u, &quad.br.texCoords.v, u_center, v_center, uv_rotation);
		}

		//UV�X�P�[�� || ���]
		if ((flags & PART_FLAG_U_SCALE) || (flags & PART_FLAG_FLIP_H))
		{
			quad.tl.texCoords.u = u_center - (u_wide * uv_scale_X * u_code);
			quad.tr.texCoords.u = u_center + (u_wide * uv_scale_X * u_code);
			quad.bl.texCoords.u = u_center - (u_wide * uv_scale_X * u_code);
			quad.br.texCoords.u = u_center + (u_wide * uv_scale_X * u_code);
		}
		if ((flags & PART_FLAG_V_SCALE) || (flags & PART_FLAG_FLIP_V))
		{
			quad.tl.texCoords.v = v_center - (v_height * uv_scale_Y * v_code);
			quad.tr.texCoords.v = v_center - (v_height * uv_scale_Y * v_code);
			quad.bl.texCoords.v = v_center + (v_height * uv_scale_Y * v_code);
			quad.br.texCoords.v = v_center + (v_height * uv_scale_Y * v_code);
		}



		//�C���X�^���X�p�[�c�̏ꍇ
		if (partData->type == PARTTYPE_INSTANCE)
		{
			//�`��
			int refKeyframe = 0;
			int refStartframe = 0;
			int refEndframe = 0;
			float refSpeed = 0;
			int refloopNum = 0;
			bool infinity = false;
			bool reverse = false;
			bool pingpong = false;
			bool independent = false;

			if (flags & PART_FLAG_INSTANCE_KEYFRAME)
			{
				refKeyframe = reader.readS16();
			}
			if (flags & PART_FLAG_INSTANCE_START)
			{
				refStartframe = reader.readS16();
			}
			if (flags & PART_FLAG_INSTANCE_END)
			{
				refEndframe = reader.readS16();
			}
			if (flags & PART_FLAG_INSTANCE_SPEED)
			{
				refSpeed = reader.readFloat();
			}
			if (flags & PART_FLAG_INSTANCE_LOOP)
			{
				refloopNum = reader.readS16();
			}
			if (flags & PART_FLAG_INSTANCE_LOOP_FLG)
			{
				int lflags = reader.readS16();
				if (lflags & INSTANCE_LOOP_FLAG_INFINITY )
				{
					//�������[�v
					infinity = true;
				}
				if (lflags & INSTANCE_LOOP_FLAG_REVERSE)
				{
					//�t�Đ�
					reverse = true;
				}
				if (lflags & INSTANCE_LOOP_FLAG_PINGPONG)
				{
					//����
					pingpong = true;
				}
				if (lflags & INSTANCE_LOOP_FLAG_INDEPENDENT)
				{
					//�Ɨ�
					independent = true;
				}
			}

			//�^�C�����C����̎��� �i��Ύ��ԁj
			int time = frameNo;

			//�Ɨ�����̏ꍇ
			if (independent)
			{
				float fdt = cocos2d::CCDirector::sharedDirector()->getAnimationInterval();
				float delta = fdt / (1.0f / _currentAnimeRef->animationData->fps);

				sprite->_liveFrame += delta;
				time = (int)sprite->_liveFrame;
			}

			//���̃C���X�^���X���z�u���ꂽ�L�[�t���[���i��Ύ��ԁj
			int	selfTopKeyframe = refKeyframe;


			int	reftime = (time * refSpeed) - selfTopKeyframe; //�J�n���猻�݂̌o�ߎ���
			if (reftime < 0) continue;							//���������������Ԃɑ��݂��Ă��Ȃ�

			int inst_scale = (refEndframe - refStartframe) + 1; //�C���X�^���X�̎�


			//�ڂ��O�������̓}�C�i�X�i���蓾�Ȃ�
			if (inst_scale <= 0) continue;
			int	nowloop = (reftime / inst_scale);	//���݂܂ł̃��[�v��

			int checkloopnum = refloopNum;

			//pingpong�̏ꍇ�ł͂Q�{�ɂ���
			if (pingpong) checkloopnum = checkloopnum * 2;

			//�������[�v�Ŗ������Ƀ��[�v�����`�F�b�N
			if (!infinity)   //�����t���O���L���ȏꍇ�̓`�F�b�N����
			{
				if (nowloop >= checkloopnum)
				{
					reftime = inst_scale - 1;
					nowloop = checkloopnum - 1;
				}
			}

			int temp_frame = reftime % inst_scale;  //���[�v���������Ȃ��C���X�^���X�A�j�����̃t���[��

			//�Q�ƈʒu�����߂�
			//���݂̍Đ��t���[���̌v�Z
			int _time = 0;
			if (pingpong && (nowloop % 2 == 1))
			{
				if (reverse)
				{
					reverse = false;//���]
				}
				else
				{
					reverse = true;//���]
				}
			}

			if (reverse)
			{
				//���o�[�X�̎�
				_time = refEndframe - temp_frame;
			}
			else{
				//�ʏ펞
				_time = temp_frame + refStartframe;
			}

			//�C���X�^���X�p�����[�^��ݒ�
			sprite->_ssplayer->set_InstanceAlpha(opacity);

			//�C���X�^���X�pSSPlayer�ɍĐ��t���[����ݒ肷��
			sprite->_ssplayer->setFrameNo(_time);
		}

	}


	// �e�ɕύX������Ƃ��͎������X�V����悤�t���O��ݒ肷��
	for (int partIndex = 1; partIndex < packData->numParts; partIndex++)
	{
		const PartData* partData = &parts[partIndex];
		CustomSprite* sprite = static_cast<CustomSprite*>(_parts.at(partIndex));
		CustomSprite* parent = static_cast<CustomSprite*>(_parts.at(partData->parentIndex));
		
		if (parent->_isStateChanged)
		{
			sprite->_isStateChanged = true;
		}
	}

	// �s��̍X�V
	cocos2d::CCAffineTransform trans;
	for (int partIndex = 0; partIndex < packData->numParts; partIndex++)
	{
		const PartData* partData = &parts[partIndex];
		CustomSprite* sprite = static_cast<CustomSprite*>(_parts.at(partIndex));
		

		if (sprite->_isStateChanged)
		{
			if (partIndex > 0)
			{
				//�e�̒��_�ʒu���擾
				CustomSprite* parent = static_cast<CustomSprite*>(_parts.at(partData->parentIndex));
				trans = parent->_state.trans;

				trans = CCAffineTransformTranslate(trans, parent->_state.x, parent->_state.y);
				trans = CCAffineTransformRotate(trans, CC_DEGREES_TO_RADIANS(-parent->_state.rotationZ));// Rad?
				trans = CCAffineTransformScale(trans, parent->_state.scaleX, parent->_state.scaleY);
			}
			else
			{
				//root�͏����l
				trans = cocos2d::CCAffineTransformMakeIdentity();
			}
			sprite->_state.trans = trans;
			sprite->setAdditionalTransform(trans);

			sprite->_isStateChanged = false;
		}
	}

}

void Player::checkUserData(int frameNo)
{

    if (!_delegate) return;
	
	ToPointer ptr(_currentRs->data);

	const AnimePackData* packData = _currentAnimeRef->animePackData;
	const AnimationData* animeData = _currentAnimeRef->animationData;
	const PartData* parts = static_cast<const PartData*>(ptr(packData->parts));

	if (!animeData->userData) return;
	const ss_offset* userDataIndex = static_cast<const ss_offset*>(ptr(animeData->userData));

	if (!userDataIndex[frameNo]) return;
	const ss_u16* userDataArray = static_cast<const ss_u16*>(ptr(userDataIndex[frameNo]));
	
	DataArrayReader reader(userDataArray);
	int numUserData = reader.readU16();

	for (int i = 0; i < numUserData; i++)
	{
		int flags = reader.readU16();
		int partIndex = reader.readU16();

		_userData.flags = 0;

		if (flags & UserData::FLAG_INTEGER)
		{
			_userData.flags |= UserData::FLAG_INTEGER;
			_userData.integer = reader.readS32();
		}
		else
		{
			_userData.integer = 0;
		}
		
		if (flags & UserData::FLAG_RECT)
		{
			_userData.flags |= UserData::FLAG_RECT;
			_userData.rect[0] = reader.readS32();
			_userData.rect[1] = reader.readS32();
			_userData.rect[2] = reader.readS32();
			_userData.rect[3] = reader.readS32();
		}
		else
		{
			_userData.rect[0] =
			_userData.rect[1] =
			_userData.rect[2] =
			_userData.rect[3] = 0;
		}
		
		if (flags & UserData::FLAG_POINT)
		{
			_userData.flags |= UserData::FLAG_POINT;
			_userData.point[0] = reader.readS32();
			_userData.point[1] = reader.readS32();
		}
		else
		{
			_userData.point[0] =
			_userData.point[1] = 0;
		}
		
		if (flags & UserData::FLAG_STRING)
		{
			_userData.flags |= UserData::FLAG_STRING;
			int size = reader.readU16();
			ss_offset offset = reader.readOffset();
			const char* str = static_cast<const char*>(ptr(offset));
			_userData.str = str;
			_userData.strSize = size;
		}
		else
		{
			_userData.str = 0;
			_userData.strSize = 0;
		}
		
		_userData.partName = static_cast<const char*>(ptr(parts[partIndex].name));
		_userData.frameNo = frameNo;
		
		_delegate->onUserData(this, &_userData);
	}

}
    
void Player::setPlayEndCallback(CCObject* target, SEL_PlayEndHandler selector)
{
    CC_SAFE_RELEASE(_playEndTarget);
    CC_SAFE_RETAIN(target);
    _playEndTarget = target;
    _playEndSelector = selector;
}
    

#define __PI__	(3.14159265358979323846f)
#define RadianToDegree(Radian) ((double)Radian * (180.0f / __PI__))
#define DegreeToRadian(Degree) ((double)Degree * (__PI__ / 180.0f))

void Player::get_uv_rotation(float *u, float *v, float cu, float cv, float deg)
{
	float dx = *u - cu; // ���S����̋���(X)
	float dy = *v - cv; // ���S����̋���(Y)

	float tmpX = ( dx * cosf(DegreeToRadian(deg)) ) - ( dy * sinf(DegreeToRadian(deg)) ); // ��]
	float tmpY = ( dx * sinf(DegreeToRadian(deg)) ) + ( dy * cosf(DegreeToRadian(deg)) );

	*u = (cu + tmpX); // ���̍��W�ɃI�t�Z�b�g����
	*v = (cv + tmpY);

}

//�C���X�^���X�p�[�c�̃A���t�@�l��ݒ�
void  Player::set_InstanceAlpha(int alpha)
{
	_InstanceAlpha = alpha;
}

/**
* SSPlayerDelegate
*/

SSPlayerDelegate::~SSPlayerDelegate()
{}

void SSPlayerDelegate::onUserData(Player* player, const UserData* data)
{}

/**
 * CustomSprite
 */

unsigned int CustomSprite::ssSelectorLocation = 0;
unsigned int CustomSprite::ssAlphaLocation = 0;
unsigned int CustomSprite::sshasPremultipliedAlpha = 0;

static const GLchar * ssPositionTextureColor_frag =
#include "ssShader_frag.h"

CustomSprite::CustomSprite()
	: _defaultShaderProgram(NULL)
	, _useCustomShaderProgram(false)
	, _opacity(1.0f)
	, _colorBlendFuncNo(0)
	, _liveFrame(0.0f)
	, _hasPremultipliedAlpha(0)
{}

CustomSprite::~CustomSprite()
{}

cocos2d::CCGLProgram* CustomSprite::getCustomShaderProgram()
{
	using namespace cocos2d;

	static CCGLProgram* p = NULL;
	static bool constructFailed = false;
	if (!p && !constructFailed)
	{
		p = new CCGLProgram();
		p->initWithVertexShaderByteArray(
			ccPositionTextureColor_vert,
			ssPositionTextureColor_frag);
		p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

		if (!p->link())
		{
			constructFailed = true;
			return NULL;
		}
		
		p->updateUniforms();
		
		ssSelectorLocation = glGetUniformLocation(p->getProgram(), "u_selector");
		ssAlphaLocation = glGetUniformLocation(p->getProgram(), "u_alpha");
		sshasPremultipliedAlpha = glGetUniformLocation(p->getProgram(), "u_hasPremultipliedAlpha");
		if (ssSelectorLocation == GL_INVALID_VALUE
		 || ssAlphaLocation == GL_INVALID_VALUE)
		{
			delete p;
			p = NULL;
			constructFailed = true;
			return NULL;
		}

		glUniform1i(ssSelectorLocation, 0);
		glUniform1f(ssAlphaLocation, 1.0f);
		glUniform1i(sshasPremultipliedAlpha, 0);
	}
	return p;
}

CustomSprite* CustomSprite::create()
{
	CustomSprite *pSprite = new CustomSprite();
	if (pSprite && pSprite->init())
	{
		pSprite->initState();
		pSprite->_defaultShaderProgram = pSprite->getShaderProgram();
		pSprite->autorelease();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return NULL;
}

void CustomSprite::changeShaderProgram(bool useCustomShaderProgram)
{
	if (useCustomShaderProgram != _useCustomShaderProgram)
	{
		if (useCustomShaderProgram)
		{
			cocos2d::CCGLProgram *shaderProgram = getCustomShaderProgram();
			if (shaderProgram == NULL)
			{
				// Not use custom shader.
				shaderProgram = _defaultShaderProgram;
				useCustomShaderProgram = false;
			}
			this->setShaderProgram(shaderProgram);
			_useCustomShaderProgram = useCustomShaderProgram;
		}
		else
		{
			this->setShaderProgram(_defaultShaderProgram);
			_useCustomShaderProgram = false;
		}
	}
}

void CustomSprite::sethasPremultipliedAlpha(int PremultipliedAlpha)
{
	_hasPremultipliedAlpha = PremultipliedAlpha;
}

bool CustomSprite::isCustomShaderProgramEnabled() const
{
	return _useCustomShaderProgram;
}

void CustomSprite::setColorBlendFunc(int colorBlendFuncNo)
{
	_colorBlendFuncNo = colorBlendFuncNo;
}

cocos2d::ccV3F_C4B_T2F_Quad& CustomSprite::getAttributeRef()
{
	return m_sQuad;
}

void CustomSprite::setOpacity(GLubyte opacity)
{
	cocos2d::CCSprite::setOpacity(opacity);
	_opacity = static_cast<float>(opacity) / 255.0f;
}


#if 1
void CustomSprite::draw(void)
{
	CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "SSSprite - draw");


	if (!_useCustomShaderProgram)
	{
		CCSprite::draw();
		return;
	}


	CCAssert(!m_pobBatchNode, "If CCSprite is being rendered by CCSpriteBatchNode, CCSprite#draw SHOULD NOT be called");

	CC_NODE_DRAW_SETUP();

	cocos2d::ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);

	if (m_pobTexture != NULL)
	{
		cocos2d::ccGLBindTexture2D(m_pobTexture->getName());
	}
	else
	{
		cocos2d::ccGLBindTexture2D(0);
	}

	glUniform1i(ssSelectorLocation, _colorBlendFuncNo);
	glUniform1f(ssAlphaLocation, _opacity);
	glUniform1i(sshasPremultipliedAlpha, _hasPremultipliedAlpha);

	//
	// Attributes
	//

	ccGLEnableVertexAttribs(cocos2d::kCCVertexAttribFlag_PosColorTex);

#define kQuadSize sizeof(m_sQuad.bl)
	long offset = (long)&m_sQuad;

	// vertex
	int diff = offsetof(cocos2d::ccV3F_C4B_T2F, vertices);
	glVertexAttribPointer(cocos2d::kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

	// texCoods
	diff = offsetof(cocos2d::ccV3F_C4B_T2F, texCoords);
	glVertexAttribPointer(cocos2d::kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

	// color
	diff = offsetof(cocos2d::ccV3F_C4B_T2F, colors);
	glVertexAttribPointer(cocos2d::kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

//	CHECK_GL_ERROR_DEBUG();


#if CC_SPRITE_DEBUG_DRAW == 1
	// draw bounding box
	CCPoint vertices[4] = {
		ccp(m_sQuad.tl.vertices.x, m_sQuad.tl.vertices.y),
		ccp(m_sQuad.bl.vertices.x, m_sQuad.bl.vertices.y),
		ccp(m_sQuad.br.vertices.x, m_sQuad.br.vertices.y),
		ccp(m_sQuad.tr.vertices.x, m_sQuad.tr.vertices.y),
	};
	ccDrawPoly(vertices, 4, true);
#elif CC_SPRITE_DEBUG_DRAW == 2
	// draw texture box
	CCSize s = this->getTextureRect().size;
	CCPoint offsetPix = this->getOffsetPosition();
	CCPoint vertices[4] = {
		ccp(offsetPix.x,offsetPix.y), ccp(offsetPix.x+s.width,offsetPix.y),
		ccp(offsetPix.x+s.width,offsetPix.y+s.height), ccp(offsetPix.x,offsetPix.y+s.height)
	};
	ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW

	CC_INCREMENT_GL_DRAWS(1);

	CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
}
#endif

void CustomSprite::setFlippedX(bool flip)
{
	_flipX = flip;
}
void CustomSprite::setFlippedY(bool flip)
{
	_flipY = flip;
}
bool CustomSprite::isFlippedX()
{
	return (_flipX);
}
bool CustomSprite::isFlippedY()
{
	return (_flipY);
}


};