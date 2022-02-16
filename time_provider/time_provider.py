#!/usr/bin/env python3

import iterm2
from datetime import datetime
import signal
import asyncio
import copy 
import concurrent.futures


def get_time():
    now=datetime.now()
    string=now.strftime('%m月%d日 %H:%M:%S')
    return string


async def main(connection):
    app=await iterm2.async_get_app(connection)
    
    print("call")
    loop = asyncio.get_running_loop()
    sessions=[]
    async with iterm2.NewSessionMonitor(connection) as mon:
        while True:
            session_id=await mon.async_get()
            session=app.get_session_by_id(session_id)
            #print(session)
            sessions.append(session)
            signal.setitimer(signal.ITIMER_REAL, 1, 1)
            async def set_time():
                for session in sessions:
                    try:
                        str_time=get_time()
                        await session.async_set_variable("user.now_date_time",str_time)
                    except:
                        sessions.remove(session)
            loop.add_signal_handler(signal.SIGALRM,lambda : asyncio.create_task(set_time()))

    

iterm2.run_forever(main)
