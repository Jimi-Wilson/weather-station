import { ModeToggle } from '@/components/ModeToggle';
import { IconLink } from '@/components/IconLink';

import GithubDarkUrl from '@/assets/github-mark-white.svg?url';
import GithubLightUrl from '@/assets/github-mark.svg?url';


export function Navbar() {
    return (
        <header className="sticky top-0 z-50 w-full bg-background/95 backdrop-blur supports-[backdrop-filter]:bg-background/60">
            <div className="container mx-auto flex h-14 items-center justify-between">
                <div className="mr-4 flex">
                    <a href="" className="flex items-center">
                        <span className="font-bold">Jimi Wilson</span>
                    </a>
                </div>
                <div className="flex items-center justify-end gap-2">
                    <IconLink
                        url="https://github.com/Jimi-Wilson"
                        text="Github"
                        mode="responsive"
                        lightIconUrl={GithubLightUrl}
                        darkIconUrl={GithubDarkUrl}
                        altText="Github Icon"
                    />
                    <ModeToggle />
                </div>
            </div>
        </header>
    );
}