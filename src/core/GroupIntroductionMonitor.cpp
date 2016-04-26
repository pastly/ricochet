#include "GroupIntroductionMonitor.h"

#include <QDebug>
#include <QTimer>

using Protocol::Data::GroupMeta::Introduction;

GroupIntroductionMonitor::GroupIntroductionMonitor(Introduction introduction, QHash<QString, GroupMember*> members, QObject *parent)
    : QObject(parent)
{
    auto invitee = QString::fromStdString(introduction.invite_response().author());
    foreach (auto member, members)
        if (!member->isSelf() && member->ricochetId() != invitee)
            m_outstandingMembers.insert(member->ricochetId(), member);
    if (m_outstandingMembers.size() < 1) {
        emit introductionMonitorDone(this, true);
    }
    m_introduction = introduction;
    QTimer::singleShot(10*1000, this, &GroupIntroductionMonitor::onTimeout);
}

void GroupIntroductionMonitor::onTimeout()
{
    emit introductionMonitorDone(this, (m_outstandingMembers.size() < 1));
}

void GroupIntroductionMonitor::onIntroductionResponseReceived(Protocol::Data::GroupMeta::IntroductionResponse introductionResponse)
{
    if (!introductionResponse.accepted())
        return;
    if (!introductionResponse.message_id() != introduction().message_id())
        return;
    if (m_outstandingMembers.contains(QString::fromStdString(introductionResponse.author()))) {
        m_outstandingMembers.remove(QString::fromStdString(introductionResponse.author()));
    }
    if (m_outstandingMembers.size() < 1) {
        emit introductionMonitorDone(this, true);
    }
}
