#include "atlas.h"

Atlas::Atlas() {}


void Atlas::Clear(){
    frame_list_.clear();
}

int Atlas::GetSize(){
    return frame_list_.size();
}

QPixmap* Atlas::GetFrame(int idx){
    if(idx<0 || idx >= GetSize()){
        return nullptr;
    }
    return &frame_list_[idx];
}

void Atlas::AddFrame(const QPixmap &frame){
    frame_list_.push_back(frame);
}


void Atlas::Load(QString path,int num){
    Clear();
    frame_list_.resize(num);


}
