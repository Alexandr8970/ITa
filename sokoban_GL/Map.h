#ifndef _MAP_H_
#define _MAP_H_

#include "common.h"

// http://www.sokobano.de/wiki/index.php?title=Level_format
//'#' - wall
//'@' - loader
//'+' - player on goal square
//'$' - box
//'*' - Box on goal square
//'.' - Goal square 
//' ' - empty
struct sLevelData
{
    int                 mLevel;
    int                 mRows;
    int                 mColumns;
    int                 mLoaderRowPos;
    int                 mLoaderColPos;
    std::vector<char>   mData;
};

class Map
{
public:
    enum eMoveDirectiron
    {
         MD_UP = 0
        ,MD_DOWN
        ,MD_RIGHT
        ,MD_LEFT
    };

    enum eMapValue
    {
         MV_WALL            = '#'
        ,MV_LOADER          = '@'
        ,MV_LOADER_ON_GOAL  = '+'
        ,MV_BOX             = '$'
        ,MV_BOX_ON_GOAL     = '*'
        ,MV_GOAL            = '.'
        ,MV_EMPTY           = ' '
    };

public:
    Map();
    ~Map();

    void loadLevels(const std::string &aMapPath);
    void setLevel(int aLevel);
    void setViewSize(const int &aWidth, const int &aHeight);
    void loaderMove(eMoveDirectiron aDirection);
    const int &getStartLevel() const;
    const int &getEndLevel() const;

    void draw();

private:
    bool _isEmpty(const int &aColumn, const int &aRow);
    bool _isMoving(eMoveDirectiron aDirection);
    void _loader_move(eMoveDirectiron aDirection);

    void _load_from_file(const std::string &aMapPath);
    void _read_level_info(ifstream &aFile, sLevelData &aLevel);
    void _read_level_data(ifstream &aFile, sLevelData &aLevel);
    void _draw_map_object(const int &aColumn, const int &aRow, eMapValue aMapObjType);

    void _load_textures();
    void _load_texture_box();
    void _load_texture_box_on_goal();
    void _load_texture_loader_on_goal();
    void _load_texture_wall();
    void _load_texture_goal();
    void _load_texture_empty();
    void _load_texture_loader();
    void _load_texture_fon();

private:
    int mCurrentLevel;
    int mStartLevel;
    int mEndLevel;
    int mLoaderPosRow;    // Y
    int mLoaderPosColumn; // X
    int mViewWidth;
    int mViewHeight;

    int mInternalViewWidth;
    int mInternalViewHeight;
    int mMapObjectSize;

    int mStartDrawX;
    int mStartDrawY;

    std::vector<sLevelData> mLevelData;
    std::vector<sLevelData> mInitLevelData;

    GLuint texture_box;
    GLuint texture_box_on_goal;
    GLuint texture_loader_on_goal;
    GLuint texture_wall;
    GLuint texture_goal;
    GLuint texture_empty;
    GLuint texture_loader;
    GLuint texture_fon;
};

#endif /* _MAP_H_ */
