from datetime import date, timedelta
import random

random.seed(348)

senders = [
    "Boss",
    "ImportantPerson",
    "Peer",
    "OtherPerson",
]

subjects = [
    "Important",
    "Health Insurance Enrollment",
    "Never Mind",
    "Project Update",
    "Meeting Reminder",
    "Quarterly Review",
    "Schedule Change",
    "Action Required",
    "Team Lunch",
    "Budget Discussion",
    "Assignment Feedback",
    "Follow Up",
    "New Policy",
    "Important, Mind",
    "Please Review",
]

start_date = date(2024, 1, 1)
end_date = date(2025, 12, 31)
date_range = (end_date - start_date).days

with open("emails.txt", "w", encoding="utf-8") as file:
    for _ in range(3000):
        sender = random.choice(senders)
        subject = random.choice(subjects)
        email_date = start_date + timedelta(
            days=random.randint(0, date_range)
        )

        file.write(
            f"EMAIL {sender},{subject},"
            f"{email_date.strftime('%m-%d-%Y')}\n"
        )

print("Created emails.txt with 300 email records.")

