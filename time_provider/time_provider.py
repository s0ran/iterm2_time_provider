#!/usr/bin/env python3

import iterm2
from datetime import datetime
import signal
import asyncio
import copy 
import concurrent.futures


def get_time():
    now=datetime.now()
    string=now.strftime('%m月%d日 %H:%M')
    return string

def set_delay():
    now=datetime.now()
    eps=10**-2
    return 60-now.second-now.microsecond*(10**-6)+eps


async def main(connection):
    app=await iterm2.async_get_app(connection)
    loop = asyncio.get_running_loop()
    sessions=[]
    async def set_time():
        #print(signal.getitimer(signal.ITIMER_REAL))     
        for session in sessions:
            try:
                str_time=get_time()
                await session.async_set_variable("user.now_date_time",str_time)
            except:
                sessions.remove(session)
        return str_time

    async with iterm2.NewSessionMonitor(connection) as mon:
        while True:
            session_id=await mon.async_get()
            session=app.get_session_by_id(session_id)
            sessions.append(session)
            await set_time()
            if signal.getitimer(signal.ITIMER_REAL)==(0,0):# ずれるなら条件変えて同期とる。
                signal.setitimer(signal.ITIMER_REAL, set_delay(), 60)
            loop.add_signal_handler(signal.SIGALRM,lambda : asyncio.create_task(set_time()))


    

iterm2.run_forever(main)
