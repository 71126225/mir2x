function main()
    uidRemoteCall(getNPCharUID('比奇县_0', '图书管理员_1'), getUID(), getQuestName(),
    [[
        local questUID, questName = ...
        local questPath = {SYS_EPQST, questName}

        return setQuestHandler(questName,
        {
            [SYS_CHECKACTIVE] = function(uid)
                return uidRemoteCall(uid, [=[ return getQuestState('比奇商会') ]=]) == 'quest_persuade_pharmacist_and_librarian'
            end,

            [SYS_ENTER] = function(uid, value)
                uidPostXML(uid, questPath,
                [=[
                    <layout>
                        <par>早以前这里就是武林人士聚集的地方，呵呵。</par>
                        <par>你看起来也像个习武之人，来这里有什么事情吗？</par>
                        <par></par>
                        <par><event id="npc_discuss_1">我为了劝您加入比奇商会而来此的！</event></par>
                    </layout>
                ]=])
            end,

            npc_discuss_1 = function(uid, value)
                uidPostXML(uid, questPath,
                [=[
                    <layout>
                        <par>比奇商会？</par>
                        <par>啊！啊！知道了！是那个叫做王大人的创办的商人联合会吧！可是我已经加入了崔大夫创办的传奇商会，还是去别的地方试试吧！</par>
                        <par></par>
                        <par><event id="npc_discuss_2">也就是说无论如何都不行吗？</event></par>
                    </layout>
                ]=], SYS_EXIT)
            end,

            npc_discuss_2 = function(uid, value)
                uidPostXML(uid, questPath,
                [=[
                    <layout>
                        <par>无论如何？那倒也不是。</par>
                        <par>如果你能为我办点事情的话，我也不是不能考虑加入比奇商会的。</par>
                        <par></par>
                        <par><event id="npc_discuss_3">要我帮你做什么事儿才行呢？</event></par>
                    </layout>
                ]=])
            end,

            npc_discuss_3 = function(uid, value)
                uidPostXML(uid, questPath,
                [=[
                    <layout>
                        <par>其实最近我正在编撰记录比奇省地理和历史的书籍。如果想要写好这本书的话必然要从各种各样的人那里收集关于比奇省的资料和信息，可是唯独比奇省的卫士们那里不与我合作啊！</par>
                        <par>不管怎么样你也是武林人士，可能和他们能够有通融的地方，所以这就是我要拜托你的事情！值班卫士反正也不能和别人说话，所以希望你能替我去那儿找那些休班卫士从他们那里收集关于比奇省历史的故事。如果你能做到的话，我会听你的劝告加入比奇商会的。</par>
                        <par></par>
                        <par><event id="npc_accept">也许我可以试试？</event></par>
                    </layout>
                ]=])
            end,

            npc_accept = function(uid, value)
                uidPostXML(uid, questPath,
                [=[
                    <layout>
                        <par>哦？你答应我的请求了？</par>
                        <par>那太好了，我等着你的好消息！</par>
                        <par></par>
                        <par><event id="%s">好的！</event></par>
                    </layout>
                ]=], SYS_EXIT)
                uidRemoteCall(questUID, uid, [=[ setUIDQuestState(..., SYS_ENTER) ]=])
            end,
        })
    ]])

    setQuestFSMTable(
    {
        [SYS_ENTER] = function(uid, value)
            setupNPCQuestBehavior('比奇县_0', '图书管理员_1', uid,
            [[
                return getQuestName()
            ]],
            [[
                local questName = ...
                local questPath = {SYS_EPUID, questName}
                return
                {
                    [SYS_ENTER] = function(uid, value)
                        uidPostXML(uid, questPath,
                        [=[
                            <layout>
                                <par>你看起来还没有听到休班卫士的全部故事啊？可以在比奇省内转转就可以找到休班卫士！</par>
                                <par></par>
                                <par><event id="%s">好的</event></par>
                            </layout>
                        ]=], SYS_EXIT)
                    end,
                }
            ]])

            setupNPCQuestBehavior('比奇县_0', '休班卫士_1', uid,
            [[
                return getUID(), getQuestName()
            ]],
            [[
                local questUID, questName = ...
                local questPath = {SYS_EPUID, questName}
                return
                {
                    [SYS_ENTER] = function(uid, value)
                        uidPostXML(uid, questPath,
                        [=[
                            <layout>
                                <par>这鬼天气真，是让人心烦气躁，要是能来口酒润润嗓子多好啊！</par>
                                <par>随便打扰别人真是没礼貌，你有什么事情吗？</par>
                                <par></par>
                                <par><event id="">你是否知道比奇省的历史？</event></par>
                            </layout>
                        ]=], SYS_EXIT)
                    end,

                    npc_discuss_1 = function(uid, value)
                        uidPostXML(uid, questPath,
                        [=[
                            <layout>
                                <par>嗨！你这家伙!求别人办事情至少要应该有点诚意吧？真是个不明事理的家伙啊！</par>
                                <par>唔, 嗓子有点干，想去酒店喝杯酒啊！咦？这个月的薪水已经全都喝酒花干净了！钱可真不经花啊！</par>
                                <par></par>
                                <par><event id="npc_decide_to_buy_soju">退出</event></par>
                            </layout>
                        ]=], SYS_EXIT)
                    end,

                    npc_decide_to_buy_soju = function(uid, value)
                        uidRemoteCall(questUID, uid, [=[ setUIDQuestState(..., 'quest_give_soju') ]=])
                    end,
                }
            ]])
        end,

        quest_give_soju = function(uid, value)
            setupNPCQuestBehavior('比奇县_0', '休班卫士_1', uid,
            [[
                return getUID(), getQuestName()
            ]],
            [[
                local questUID, questName = ...
                local questPath = {SYS_EPUID, questName}
                return
                {
                    [SYS_ENTER] = function(uid, value)
                        if uidRemoteCall(uid, [=[ hasItem(getItemID('烧酒', 0, 1)) ]=]) then
                            uidPostXML(uid, questPath,
                            [=[
                                <layout>
                                    <par>啊！又是你，你能不能离我远...等等！这是烧酒的味道，好香啊！</par>
                                    <par>这位%s，能不能给我喝口酒啊！</par>
                                    <par></par>
                                    <par><event id="npc_give_soju">拿去吧！</event></par>
                                    <par><event id="npc_deny_soju">不愿意。</event></par>
                                </layout>
                            ]=], uidRemoteCall(uid, [=[ return getGender() ]=]) and '少侠' or '姑娘')
                        else
                            uidPostXML(uid, questPath,
                            [=[
                                <layout>
                                    <par>啊！又是你，你能不能离我远点！？大热天的为什么要三番五次地惹人烦呢？</par>
                                    <par>要是能有口酒润润嗓子就好啦！</par>
                                    <par></par>
                                    <par><event id="%s">退出</event></par>
                                </layout>
                            ]=], SYS_EXIT)
                        end
                    end,

                    npc_give_soju = function(uid, value)
                        uidPostXML(uid, questPath,
                        [=[
                            <layout>
                                <par>啊哈哈哈，真是谢谢你！</par>
                                <par>咕噜咕噜，还是烧酒的味道棒啊！咕噜咕噜，炎热的午后能痛快地喝一顿真是神仙般的快活啊！</par>
                                <par></par>
                                <par><event id="npc_ask_info">不知道你能否告知比奇省的历史？</event></par>
                            </layout>
                        ]=])
                    end,

                    npc_deny_soju = function(uid, value)
                        uidPostXML(uid, questPath,
                        [=[
                            <layout>
                                <par>你是不愿意给我喝呢，还是酒已经被喝光了？</par>
                                <par>真扫兴！</par>
                                <par></par>
                                <par><event id="%s">退出</event></par>
                            </layout>
                        ]=], SYS_EXIT)
                    end,

                    npc_ask_info = function(uid, value)
                        uidPostXML(uid, questPath,
                        [=[
                            <layout>
                                <par>比奇省的历史？嗯？唔...</par>
                                <par>说起比奇的由来这要追溯到几百年之前啦！比奇产生之前，西方有几个国家，由于被叫做内日和半兽人的怪物种族袭击一直都处于危险之中，处于威机之中的这几个国家停止了相互之间的战争，协力与怪物们抗争，最后终于赶走了怪物们，但是也全部受到了重创，怪物们的威胁仍然没有完全解除。</par>
                                <par>于是这几个国家协力出兵去讨伐怪物们的根据地，那个地方就是比奇地区！</par>
                                <par>这些够了吧！</par>
                                <par></par>
                                <par><event id="npc_ask_more_info">你还知道更多的信息吗？</event></par>
                            </layout>
                        ]=])
                    end,

                    npc_ask_more_info = function(uid, value)
                        uidPostXML(uid, questPath,
                        [=[
                            <layout>
                                <par>看在烧酒的面子上，我知道的就这些啦！想知道的更多，你也可以去问问其他的卫士。</par>
                                <par></par>
                                <par><event id="npc_done_query_guard_1">好的！</event></par>
                            </layout>
                        ]=])
                    end,

                    npc_done_query_guard_1 = function(uid, value)
                        uidRemoteCall(questUID, uid, [=[ addUIDQuestFlag(..., 'flag_done_query_guard_1') ]=])
                    end,
                }
            ]])

            setupNPCQuestBehavior('比奇县_0', '休班卫士_2', uid,
            [[
                return getUID(), getQuestName()
            ]],
            [[
                local questUID, questName = ...
                local questPath = {SYS_EPUID, questName}
                return
                {
                    [SYS_ENTER] = function(uid, value)
                        uidPostXML(uid, questPath,
                        [=[
                            <layout>
                                <par>听说有人找我，是你吗？</par>
                                <par>鄙人就是崔某，有什么事儿吗？</par>
                                <par></par>
                                <par><event id="npc_ask_info">我想知道有关比奇省的历史。</event></par>
                            </layout>
                        ]=])
                    end,

                    npc_ask_info = function(uid, value)
                        uidPostXML(uid, questPath,
                        [=[
                            <layout>
                                <par>说起比奇省的历史...</par>
                                <par>知道吗？我们的祖先就是讨伐半兽人族地区而派遣出的远征队啊！我们的祖先经过残酷的战斗终于击溃了怪物们。一想到只要再继续坚持战斗一下就可以把怪物们斩草除根，然后可以回到故乡，就都非常高兴。</par>
                                <par>可是没想到这时突然发生了始料未及的灾难。这里发生了大地震。原本可以翻过山脉回到家乡的路由于这次大地震导致地壳变动，完全的被隔断了！有的人痛哭流涕，有的人茫然失措。所有人都慌了手脚。</par>
                                <par>但是一位优秀的将领重新振作精神，开始在这个地区寻找求生之路。他指挥着他的部下们在赶走半兽人族的地区找到了一片肥沃的土地建立了新的城市。这就是现在的比奇省。</par>
                                <par>好了，我已经把知道的基本上全都告诉你啦...我也要走啦！</par>
                                <par></par>
                                <par><event id="npc_done_query_guard_2">谢谢！</event></par>
                            </layout>
                        ]=], SYS_EXIT)
                    end,

                    npc_done_query_guard_2 = function(uid, value)
                        uidRemoteCall(questUID, uid, [=[ addUIDQuestFlag(..., 'flag_done_query_guard_2') ]=])
                    end,
                }
            ]])

            setupNPCQuestBehavior('比奇县_0', '休班卫士_3', uid,
            [[
                return getUID(), getQuestName()
            ]],
            [[
                local questUID, questName = ...
                local questPath = {SYS_EPUID, questName}
                return
                {
                    [SYS_ENTER] = function(uid, value)
                        uidPostXML(uid, questPath,
                        [=[
                            <layout>
                                <par>是你啊？四处打听比奇省历史的人？</par>
                                <par></par>
                                <par><event id="npc_check_flags">是的啊！</event></par>
                            </layout>
                        ]=])
                    end,

                    npc_check_flags = function(uid, value)
                        local done_guard_1 = uidRemoteCall(questUID, uid, [=[ hasUIDQuestFlag(..., 'flag_done_query_guard_1') ]=])
                        local done_guard_2 = uidRemoteCall(questUID, uid, [=[ hasUIDQuestFlag(..., 'flag_done_query_guard_2') ]=])

                        if done_guard_1 and done_guard_2 then
                            uidPostXML(uid, questPath,
                            [=[
                                <layout>
                                    <par>哦！你也是来问我关于比奇省历史的吗？</par>
                                    <par></par>
                                    <par><event id="npc_ask_info">是的, 请您讲讲比奇省历史的故事吧！</event></par>
                                </layout>
                            ]=])
                        else
                            uidPostXML(uid, questPath,
                            [=[
                                <layout>
                                    <par>喂！我可是卫士中资历最深的！你先去跟其他的人打听之后再来找我吧！</par>
                                    <par>不能让人小瞧了我...</par>
                                    <par></par>
                                    <par><event id="%s">退出</event></par>
                                </layout>
                            ]=], SYS_EXIT)
                        end
                    end,

                    npc_ask_info = function(uid, value)
                        uidPostXML(uid, questPath,
                        [=[
                            <layout>
                                <par>我什么都不知道，你还是去问别人吧！</par>
                                <par></par>
                                <par><event id="npc_give_100_gold">给他100金币</event></par>
                                <par><event id="npc_give_1000_gold">给他1000金币</event></par>
                                <par><event id="%s">不询问他</event></par>
                            </layout>
                        ]=], SYS_EXIT)
                    end,

                    npc_give_100_gold = function(uid, value)
                        uidPostXML(uid, questPath,
                        [=[
                            <layout>
                                <par>嗨哎！？这是干吗？</par>
                                <par></par>
                                <par><event id="npc_give_100_gold">请以后买点酒喝什么的吧！</event></par>
                            </layout>
                        ]=])
                    end,

                    npc_give_1000_gold = function(uid, value)
                        uidPostXML(uid, questPath,
                        [=[
                            <layout>
                                <par>你...你这是做什么？竟敢和保护比奇省治安的我开这种玩笑？</par>
                                <par>看来和你是做不了朋友了！要和我比试比试吗？</par>
                                <par>我长这么大还是头一次受到这种污辱！</par>
                                <par></par>
                                <par><event id="npc_angry_1">你千万别误会啊！不是这个意思！</event></par>
                            </layout>
                        ]=])
                    end,

                    npc_angry_1 = function(uid, value)
                        uidPostXML(uid, questPath,
                        [=[
                            <layout>
                                <par>你还狡辩什么啊？你这个%s！</par>
                                <par></par>
                                <par><event id="npc_angry_2">你千万不要误会呀！</event></par>
                            </layout>
                        ]=], uidRemoteCall(uid, [=[ return getGender() ]=]) and '混小子' or '混丫头')
                    end,

                    npc_angry_2 = function(uid, value)
                        uidPostXML(uid, questPath,
                        [=[
                            <layout>
                                <par>哼！呵呵...没有别的意思！真的吗？</par>
                                <par></par>
                                <par><event id="npc_angry_3">对不起是我错了，请原谅！</event></par>
                            </layout>
                        ]=])
                    end,

                    npc_angry_3 = function(uid, value)
                        uidPostXML(uid, questPath,
                        [=[
                            <layout>
                                <par>唉！没办法，谁让我年纪大来着呢，原谅一下你吧！这里有1钱！</par>
                                <par>快去买5瓶烧酒来，喝了酒才能消了我的肚子里的火气。别忘了把找还的零钱带回来！</par>
                                <par></par>
                                <par><event id="npc_angry_4">对不起是我错了，请原谅！</event></par>
                            </layout>
                        ]=])
                    end,

                    npc_give_info = function(uid, value)
                        uidPostXML(uid, questPath,
                        [=[
                            <layout>
                                <par>祖先们修建了这比奇省和里面的城镇村庄之后，就开始反复的在周边勘查并拓展自己的根据地。但是这附近值得利用的土地非常的少。很难足够的支持别的地方的农事生产需要。</par>
                                <par>随着人口逐渐的增加，人们为了寻找更加宽阔的土地和更多的资源开始拓宽自己的领土。于是人们向沃玛、蛇谷、盟众一步一步的扩大土地，开拓没有人烟到达过的沼泽地，也遇到了生活在森林、灌木丛和山洞中其它各种各样的怪物并与它们发生战争，就这样一点一点的扩大了领土，可以说每一寸土地都是用鲜血换来的啊！</par>
                                <par>尽管我们现在占据了宽广的领土，但在比奇土地上各处都仍存在着怪物的势力，加上大部分地区全都是深山和茂密的灌木丛，仍然会发生种种阻断村庄之间道路的事情...</par>
                                <par>唔…这已经是我所知道的全部故事啦！</par>
                                <par></par>
                                <par><event id="npc_done_query_guard_3">谢谢！</event></par>
                            </layout>
                        ]=])
                    end,

                    npc_done_query_guard_3 = function(uid, value)
                        uidRemoteCall(questUID, uid, [=[ addUIDQuestFlag(..., 'flag_done_query_guard_3') ]=])
                    end,
                }
            ]])
        end,

        quest_give_guard_3_soju = function(uid, value)
            setupNPCQuestBehavior('比奇县_0', '休班卫士_3', uid,
            [[
                return getUID(), getQuestName()
            ]],
            [[
                local questUID, questName = ...
                local questPath = {SYS_EPUID, questName}
                return
                {
                    [SYS_ENTER] = function(uid, value)
                        local hasSoju    = uidRemoteCall(uid, [=[ hasItem(getItemID('烧酒'), 0, 1) ]=])
                        local hasSoju_x5 = uidRemoteCall(uid, [=[ hasItem(getItemID('烧酒'), 0, 5) ]=])

                        if not hasSoju then
                            uidPostXML(uid, questPath,
                            [=[
                                <layout>
                                    <par>快去买啊！1个金币不够吗？</par>
                                    <par></par>
                                    <par><event id="%s">退出</event></par>
                                </layout>
                            ]=], SYS_EXIT)

                        elseif not hasSoju_x5 then
                            uidPostXML(uid, questPath,
                            [=[
                                <layout>
                                    <par>臭小子，我是说五瓶，快去再买点！</par>
                                    <par>竟敢不听我的！</par>
                                    <par></par>
                                    <par><event id="%s">退出</event></par>
                                </layout>
                            ]=], SYS_EXIT)

                        else
                            uidPostXML(uid, questPath,
                            [=[
                                <layout>
                                    <par>呵！你还真买来了。</par>
                                    <par>咕噜，咕噜，啊！真是好酒，现在舒服多了。</par>
                                    <par>对了，你是找我来问什么的来着？</par>
                                    <par></par>
                                    <par><event id="npc_give_info">我想知道关于比奇省历史的事。</event></par>
                                </layout>
                            ]=], SYS_EXIT)
                        end
                    end,

                    npc_give_info = function(uid, value)
                        uidPostXML(uid, questPath,
                        [=[
                            <layout>
                                <par>哦？是嘛，好吧，我来讲给你听。</par>
                                <par>祖先们修建了这比奇省和里面的城镇村庄之后，就开始反复的在周边勘查并拓展自己的根据地。但是这附近值得利用的土地非常的少。很难足够的支持别的地方的农事生产需要。</par>
                                <par>随着人口逐渐的增加，人们为了寻找更加宽阔的土地和更多的资源开始拓宽自己的领土。于是人们向沃玛、蛇谷、盟众一步一步的扩大土地，开拓没有人烟到达过的沼泽地，也遇到了生活在森林、灌木丛和山洞中其它各种各样的怪物并与它们发生战争，就这样一点一点的扩大了领土，可以说每一寸土地都是用鲜血换来的啊！</par>
                                <par>尽管我们现在占据了宽广的领土，但在比奇土地上各处都仍存在着怪物的势力，加上大部分地区全都是深山和茂密的灌木丛，仍然会发生种种阻断村庄之间道路的事情...</par>
                                <par>唔...这已经是我所知道的全部故事啦！就说到这里吧，酒喝得很爽啊！</par>
                                <par></par>
                                <par><event id="npc_done_query_guard_3">谢谢！</event></par>
                            </layout>
                        ]=])
                    end,

                    npc_done_query_guard_3 = function(uid, value)
                        uidRemoteCall(questUID, uid, [=[ addUIDQuestFlag(..., 'flag_done_query_guard_3') ]=])
                    end,
                }
            ]])
        end,
    })
end
