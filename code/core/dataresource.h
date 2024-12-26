#ifndef DATARESOURCE_H
#define DATARESOURCE_H

#include <QObject>
#include <QTimer>
#include <QMutex>

class DataResource : public QObject {
    Q_OBJECT

public:
    // 获取单例实例
    static DataResource* instance();

    // Get 方法保持不变
    int level() const { return level_; }
    int rest_time() const { return rest_time_; }
    int score() const { return score_; }
    int target_score() const { return target_score_; }
    int rest_steps() const { return rest_steps_; }
    int total_time() const { return total_time_; }
    int total_steps() const { return total_steps_; }
    int pace() const { return pace_; }
    int elimination_times() const { return elimination_times_; }


    void set_elimination_times(int times) { elimination_times_ = times; }
    void elimination_times_add_one() { if(elimination_times_<6) ++elimination_times_; }
    void reset_elimination_times() { elimination_times_ = 1; }

    void add_score(int cube_number){ set_score(score_ + cube_number * pace_); }
    void set_total_time(int total_time) { total_time_ = total_time; }
    void set_total_steps(int total_steps) { total_steps_ = total_steps; }
    void set_pace(int pace) { pace_ = pace; }

    void set_level(int level) {
        level_ = level;
        emit level_change_signal(level);
    }

    void set_rest_time(int rest_time) {
        rest_time_ = rest_time;
        emit time_change_signal(rest_time);
    }

    void set_score(int score) {
        score_ = score;
        emit score_change_signal(score);
    }

    void set_target_score(int target_score) {
        target_score_ = target_score;
        emit target_score_change_signal(target_score);
    }

    void set_rest_steps(int rest_steps) {
        rest_steps_ = rest_steps;
        emit step_change_signal(rest_steps);
    }

    void timeout() {
        emit time_out_signal();
    }

    void reset_all_data();
    bool game_over = false;

signals:

    void time_out_signal(); //游戏结束信号
    void level_change_signal(int level);      // 改正 singal -> signal
    void time_change_signal(int rest_time);   // 改正 singal -> signal
    void score_change_signal(int score);      // 改正 socre -> score
    void target_score_change_signal(int target_score); // 改正 socre -> score
    void step_change_signal(int rest_steps);

private:
    // 禁止外部实例化和拷贝
    DataResource(QObject* parent = nullptr);
    ~DataResource() override = default;

    DataResource(const DataResource&) = delete;
    DataResource& operator=(const DataResource&) = delete;

    // 单例静态实例
    static DataResource* instance_;
    static QMutex mutex_;

    // 成员变量
    int level_ = 0;          // 关卡
    int rest_time_ = 0;      // 剩余时间
    int total_time_ = 0; //总时间
    int score_ = 0;          // 得分
    int target_score_ = 0;   // 目标分数
    int rest_steps_ = 0;     // 剩余步数
    int total_steps_ = 0;
    int pace_ = 10;



    int elimination_times_ = 1; //连续消除次数
};

#endif // DATARESOURCE_H
