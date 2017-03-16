#ifndef MODIFIED_H
#define MODIFIED_H


class Modified
{
public:
    Modified();
    virtual bool modified();
    virtual void setModified(bool);
private:
    bool mod;
};

#endif // MODIFIED_H
