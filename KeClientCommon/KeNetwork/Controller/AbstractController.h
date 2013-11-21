#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H



class AbstractController
{
public:
    int getHandler();
    int getType();
    void setType(int type);
    virtual bool CheckAvaliable(int channelID);

protected:

    void setHandler(int handle);
    AbstractController();
    virtual ~AbstractController();
    int controlHandler;
    int type;
};

#endif // ABSTRACTCONTROLLER_H
